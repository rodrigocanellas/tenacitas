/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_CLASSES_H
#define TNCT_MF4_PER_CLASSES_H

#include <cstring>
#include <fstream>
#include <utility>

#include "tnct/byte_array/classes.h"
#include "tnct/format/fmt.h"
#include "tnct/mf4/mem/classes.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::per {

template <tnct::traits::log::logger t_logger>
[[nodiscard]] std::pair<mem::id_block<t_logger>, std::int64_t>
read_id_block(std::ifstream &file, t_logger &p_logger) {
  constexpr uint8_t _str_field_size{8};
  char _id_file[_str_field_size + 1];
  std::memset(_id_file, '\0', _str_field_size + 1);
  char _id_vers[_str_field_size + 1];
  std::memset(_id_vers, '\0', _str_field_size + 1);
  char _id_prog[_str_field_size + 1];
  std::memset(_id_prog, '\0', _str_field_size + 1);
  using id_reserved_1 = std::uint32_t;
  std::uint16_t _id_ver{0xFFFF};
  using id_reserverd_2 = std::array<std::uint8_t, 30>;
  std::uint16_t _id_unfin_flags{0XFFFF};
  std::uint16_t _id_custom_unfin_flags{0xFFFF};

  constexpr std::int64_t _size{
      3 * _str_field_size + sizeof(id_reserved_1) + sizeof(decltype(_id_ver)) +
      std::tuple_size_v<id_reserverd_2> + sizeof(decltype(_id_unfin_flags)) +
      sizeof(_id_custom_unfin_flags)};

  char buf[_size];
  std::memset(&buf[0], 0, _size);

  file.seekg(0);
  if (!file.read(&buf[0], _size).good()) {
    TNCT_LOG_ERR(p_logger, "error reading id block");
    throw std::runtime_error("error reading id block");
  }

  std::memcpy(&_id_file[0], &buf[0], _str_field_size);
  std::memcpy(&_id_vers[0], &buf[_str_field_size], _str_field_size);
  std::memcpy(&_id_prog[0], &buf[2 * _str_field_size], _str_field_size);

  auto ptr{reinterpret_cast<const std::uint8_t *>(
      &buf[3 * _str_field_size + sizeof(id_reserved_1)])};

  _id_ver = byte_array::from_little<decltype(_id_ver)>(ptr);
  ptr += sizeof(_id_ver) + std::tuple_size_v<id_reserverd_2>;

  _id_unfin_flags = byte_array::from_little<decltype(_id_unfin_flags)>(ptr);
  ptr += sizeof(_id_unfin_flags);

  _id_custom_unfin_flags =
      byte_array::from_little<decltype(_id_custom_unfin_flags)>(ptr);

  auto _pair = std::make_pair(
      mem::id_block<t_logger>{p_logger, _id_file, _id_vers, _id_prog, _id_ver,
                              _id_unfin_flags, _id_custom_unfin_flags},
      _size);

  return _pair;
}

template <tnct::traits::log::logger t_logger>
[[nodiscard]] std::tuple<mem::block_id, uint64_t, std::int64_t>
read_header_section(std::ifstream &p_file, t_logger &p_logger,
                    std::int64_t p_offset_start) {
  constexpr std::int64_t _id_field_size{sizeof(mem::block_id_str) - 1};
  constexpr std::int64_t _reserved_field_size{sizeof(std::uint8_t) * 4};
  constexpr std::int64_t _length_field_size{sizeof(uint64_t)};
  constexpr std::int64_t _link_count_field_size{sizeof(uint64_t)};
  constexpr std::int64_t _total_size{_id_field_size + _reserved_field_size +
                                     _length_field_size +
                                     _link_count_field_size};

  char _buf[_total_size];
  std::memset(&_buf[0], '\0', _total_size);

  if (!p_file.seekg(p_offset_start).good()) {
    TNCT_LOG_ERR(p_logger, format::fmt("error 'seekg' to ", p_offset_start,
                                       " for header section"));
    throw std::runtime_error("error reading header section");
  }

  if (!p_file.read(_buf, _total_size).good()) {
    TNCT_LOG_ERR(p_logger, format::fmt("error reading to ", p_offset_start,
                                       " for header section of block "));
    throw std::runtime_error("error reading header section");
  }

  mem::block_id_str _id_str;
  std::memset(_id_str, '\0', _id_field_size + 1);
  std::memcpy(_id_str, &_buf[0], _id_field_size);

  auto _ptr{reinterpret_cast<const std::uint8_t *>(
      &_buf[_id_field_size + _reserved_field_size + _length_field_size])};

  // uint64_t _lenght{byte_array::from_little<decltype(_lenght)>(_ptr)};
  // _ptr += _length_field_size;

  uint64_t _link_count{byte_array::from_little<decltype(_link_count)>(_ptr)};

  const auto optional_block_id{mem::block_id_converter::to_id(_id_str)};

  if (!optional_block_id.has_value()) {
    TNCT_LOG_ERR(p_logger, format::fmt("error converting '", _id_str,
                                       "' to a mem::block_id"));
    throw std::runtime_error("error reading header section id");
  }

  auto _pair = std::make_tuple(optional_block_id.value(), _link_count,
                               std::streamsize{_total_size});

  return _pair;
}

// template <mem::block_type t_block_Id, tnct::traits::log::logger t_logger>
// struct read_link_section;

// template <tnct::traits::log::logger t_logger>

// struct read_link_section<mem::HD, t_logger> {
//   [[nodiscard]] std::pair<mem::link_section<t_logger>, std::streamsize>
//   operator()(std::ifstream &p_file, t_logger &p_logger,
//              std::streamsize p_offset_start) {
//     constexpr std::uint8_t _num_links{6};
//     constexpr std::uint8_t _buf_size{_num_links * sizeof(mem::LINK)};

//     char _buf[_buf_size];

//     std::memset(&_buf[0], 0, _buf_size);

//     p_file.seekg(p_offset_start);

//     p_file.readsome(_buf, _buf_size);

//     if (!p_file.good()) {
//       TNCT_LOG_ERR(p_logger,
//                    format::fmt("error reading link_section for block ",
//                                mem::ids_strs_[mem::HD]));
//       throw std::runtime_error("error reading link section");
//     }

//     const std::uint8_t *_ptr{reinterpret_cast<const std::uint8_t
//     *>(&_buf[0])};

//     std::pair<mem::link_section<t_logger>, std::streamsize> _result{
//         mem::HD, p_offset_start + static_cast<std::streamsize>(_buf_size)};

//     _result.first.add_link(mem::DG,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::LINK);

//     _result.first.add_link(mem::FH,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::LINK);

//     _result.first.add_link(mem::CH,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::LINK);

//     _result.first.add_link(mem::AT,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::LINK);

//     _result.first.add_link(mem::EV,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::LINK);

//     _result.first.add_link(mem::TX,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::LINK);

//     return _result;
//   }
// };

// template <> struct data_section_t<block_id::HD> {

//     static constexpr std::streamsize size{
//                                           sizeof(std::uint64_t) +
//                                           sizeof(std::int16_t) +
//                                           sizeof(std::int16_t) +
//                                           sizeof(std::uint8_t) +
//                                           sizeof(std::uint8_t) +
//                                           sizeof(std::uint8_t) +
//                                           sizeof(std::uint8_t) +
//                                           sizeof(double) + sizeof(double)};

//     data_section_t(std::streamsize offset_start, std::ifstream &file) {
//         char buf[size];
//         std::memset(&buf[0], 0, size);

//         file.seekg(offset_start);
//         if (!file.read(&buf[0], size).good()) {
//             ctw::log::err(ctw::format::fmt("error reading data section for ",
//                                            ids_strs_[HD]));
//             throw std::runtime_error("error reading data section");
//         }

//         auto ptr{reinterpret_cast<const std::uint8_t *>(&buf[0])};

//         hd_start_time_ns_ =
//             ctw::byte_array::from_little<decltype(hd_start_time_ns_)>(ptr);
//         ptr += sizeof(hd_start_time_ns_);

//         hd_tz_offset_min_ =
//             ctw::byte_array::from_little<decltype(hd_start_time_ns_)>(ptr);
//         ptr += sizeof(hd_tz_offset_min_);

//         hd_dst_offset_min_ =
//             ctw::byte_array::from_little<decltype(hd_dst_offset_min_)>(ptr);
//         ptr += sizeof(hd_dst_offset_min_);

//         hd_time_flags_ =
//             ctw::byte_array::from_little<decltype(hd_time_flags_)>(ptr);
//         ptr += sizeof(hd_time_flags_);

//         hd_flags_ = ctw::byte_array::from_little<decltype(hd_flags_)>(ptr);
//         ptr += sizeof(hd_flags_) + sizeof(hd_reserved_);

//         hd_start_angle_rad_ =
//             ctw::byte_array::from_little<decltype(hd_start_angle_rad_)>(ptr);
//         ptr += sizeof(hd_start_angle_rad_);

//         hd_start_distance_m_ =
//             ctw::byte_array::from_little<decltype(hd_start_distance_m_)>(ptr);
//     }

//     std::uint64_t Gethd_start_time_ns() const { return hd_start_time_ns_; }
//     std::int16_t Gethd_tz_offset_min() const { return hd_tz_offset_min_; }
//     std::int16_t Gethd_dst_offset_min() const { return hd_dst_offset_min_; }
//     std::uint8_t Gethd_time_flags() const { return hd_time_flags_; }
//     std::uint8_t Gethd_time_class() const { return hd_time_class_; }
//     std::uint8_t Gethd_flags() const { return hd_flags_; };
//     double Gethd_start_angle_rad() const { return hd_start_angle_rad_; }
//     double Gethd_start_distance_m() const { return hd_start_distance_m_; }

// private:
//     std::uint64_t hd_start_time_ns_;
//     std::int16_t hd_tz_offset_min_;
//     std::int16_t hd_dst_offset_min_;
//     std::uint8_t hd_time_flags_;
//     std::uint8_t hd_time_class_;
//     std::uint8_t hd_flags_;
//     std::uint8_t hd_reserved_;
//     double hd_start_angle_rad_;
//     double hd_start_distance_m_;
// };

// std::ostream &operator<<(std::ostream &out,
//                          const data_section_t<HD> &obj) {
//     out << "data section of " << ids_strs_[HD] << ": "
//         << obj.Gethd_start_time_ns() << " " << obj.Gethd_tz_offset_min() << '
//       '
//         << obj.Gethd_dst_offset_min() << ' '
//         << static_cast<uint16_t>(obj.Gethd_time_flags()) << ' '
//         << static_cast<uint16_t>(obj.Gethd_time_class()) << ' '
//         << static_cast<uint16_t>(obj.Gethd_flags()) << ' '
//         << obj.Gethd_start_angle_rad() << ' ' <<
//         obj.Gethd_start_distance_m();
//     return out;
// }

template <tnct::traits::log::logger t_logger>
mem::result mf4_reader(std::string_view file_name, t_logger &p_logger) {
  try {
    std::ifstream _file(file_name.data(), std::ios_base::binary);
    if (!_file.good()) {
      return mem::result::file_not_found;
    }

    auto [_id_block, _id_block_size] = read_id_block(_file, p_logger);
    TNCT_LOG_INF(p_logger,
                 format::fmt(_id_block, " - size = ", _id_block_size));

    auto [_header_id, _header_num_links, _heder_section_size] =
        read_header_section<t_logger>(_file, p_logger, _id_block_size);

    if (_header_id != mem::block_id::HD) {
      TNCT_LOG_ERR(
          p_logger,
          format::fmt("header should be ",
                      mem::block_id_converter::to_str(mem::block_id::HD),
                      ", but it is ",
                      mem::block_id_converter::to_str(_header_id)));
      throw std::runtime_error("error reading header section of HEADER block");
    }
    TNCT_LOG_INF(p_logger,
                 format::fmt("reader header section for block ",
                             mem::block_id_converter::to_str(_header_id),
                             ", it has ", _header_num_links, " links and size ",
                             _heder_section_size));

    // auto [_link_section, _link_section_size] =
    //     read_link_section<mem::HD, t_logger>()(
    //         _file, p_logger, _id_block_size + _header_section_size);
    // TNCT_LOG_INF(p_logger, tnct::format::fmt(_link_section));

    // tnct::mf4::mem::LinkSection<tnct::mf4::mem::HD> link_section(
    //     tnct::mf4::mem::Block<tnct::mf4::mem::ID>::size +
    //         tnct::mf4::mem::HeaderSection<tnct::mf4::mem::HD>::size,
    //     file);
    // TNCT_LOG_INF(p_logger, tnct::format::fmt(link_section));

    // tnct::mf4::mem::DataSection<tnct::mf4::mem::HD> data_section(
    //     tnct::mf4::mem::Block<tnct::mf4::mem::ID>::size +
    //         tnct::mf4::mem::HeaderSection<tnct::mf4::mem::HD>::size +
    //         tnct::mf4::mem::LinkSection<tnct::mf4::mem::HD>::size,
    //     file);
    // TNCT_LOG_INF(p_logger, tnct::format::fmt(data_section));
    return mem::result::ok;
  } catch (std::exception &ex) {
    std::cout << "ERROR: " << ex.what() << '\n';
  } catch (...) {
    std::cout << "ERROR unknow" << '\n';
  }
  return mem::result::error_reading;
}

} // namespace tnct::mf4::per

#endif
