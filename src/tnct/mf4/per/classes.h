/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_CLASSES_H
#define TNCT_MF4_PER_CLASSES_H

#include <cstring>
#include <fstream>
#include <functional>
#include <source_location>
#include <utility>

#include "tnct/byte_array/classes.h"
#include "tnct/format/fmt.h"
#include "tnct/mf4/mem/classes.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4 {

template <tnct::traits::log::logger t_logger>
void log_and_throw(
    t_logger &p_logger, std::string_view p_str,
    std::source_location p_location = std::source_location::current()) {
  p_logger.err(p_str, p_location);
  throw std::runtime_error(p_str.data());
}

} // namespace tnct::mf4

namespace tnct::mf4::per {

using amount_to_read = std::uint64_t;
using offset = std::int64_t;
using field_size = std::uint8_t;

static constexpr amount_to_read header_section_size{24};

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

  constexpr amount_to_read _amount_to_read{
      3 * _str_field_size + sizeof(id_reserved_1) + sizeof(decltype(_id_ver)) +
      std::tuple_size_v<id_reserverd_2> + sizeof(decltype(_id_unfin_flags)) +
      sizeof(_id_custom_unfin_flags)};

  char buf[_amount_to_read];
  std::memset(&buf[0], 0, _amount_to_read);

  if (!file.seekg(0).good()) {
    log_and_throw(p_logger, "could not seekg to 0");
  }
  if (!file.read(&buf[0], _amount_to_read).good()) {
    log_and_throw(p_logger, "error reading id block");
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
      static_cast<offset>(_amount_to_read));

  return _pair;
}

template <tnct::traits::log::logger t_logger>
[[nodiscard]] std::tuple<mem::block_id, amount_to_read, std::size_t>
read_header_section(std::ifstream &p_file, t_logger &p_logger,
                    offset p_offset_start) {
  constexpr field_size _reserved_field_size{sizeof(std::uint8_t) * 4};

  char _buf[header_section_size];
  std::memset(&_buf[0], '\0', header_section_size);

  if (!p_file.seekg(p_offset_start).good()) {
    log_and_throw(p_logger, format::fmt("error 'seekg' to ", p_offset_start,
                                        " for header section"));
  }

  if (!p_file.read(_buf, header_section_size).good()) {
    log_and_throw(p_logger, format::fmt("error reading to ", p_offset_start,
                                        " for header section of block "));
  }

  mem::block_id_str _id_str;
  std::memset(_id_str, '\0', sizeof(mem::block_id_str));
  std::memcpy(_id_str, &_buf[0], sizeof(mem::block_id_str) - 1);

  auto _ptr{reinterpret_cast<const std::uint8_t *>(
      &_buf[sizeof(mem::block_id_str) - 1 + _reserved_field_size])};

  amount_to_read _length{byte_array::from_little<decltype(_length)>(_ptr)};
  _ptr += sizeof(decltype(_length));

  std::size_t _link_count{byte_array::from_little<decltype(_link_count)>(_ptr)};

  const auto optional_block_id{mem::block_id_converter::to_id(_id_str)};

  if (!optional_block_id.has_value()) {
    log_and_throw(p_logger, format::fmt("error converting '", _id_str,
                                        "' to a mem::block_id"));
  }

  auto _pair = std::make_tuple(optional_block_id.value(), _length, _link_count);

  return _pair;
}

template <mem::block_id t_block_id> struct block_reader_t {
  template <tnct::traits::log::logger t_logger>
  void operator()(std::ifstream &p_file, t_logger &p_logger,
                  int64_t p_offset_start, amount_to_read p_block_size,
                  std::size_t p_num_links,
                  std::optional<mem::block_ref> p_parent,
                  std::size_t p_level = 0) {

    const amount_to_read size_to_read{p_block_size - header_section_size};

    if (!p_file.seekg(p_offset_start).good()) {
      log_and_throw(p_logger,
                    format::fmt("error seekg for ", p_offset_start, " bytes"));
    }

    std::vector<char> _buf(size_to_read, 0);
    if (!p_file.read(_buf.data(), size_to_read).good()) {
      log_and_throw(p_logger, format::fmt("error reading ", size_to_read,
                                          " bytes for header block "));
    }

    mem::block_t<t_block_id> _block{p_offset_start, p_parent};

    auto _ptr = reinterpret_cast<const std::uint8_t *>(&_buf[0]);
    {
      for (decltype(p_num_links) _i = 0; _i < p_num_links; ++_i) {
        offset _offset{
            byte_array::from_little<offset>(_ptr + _i * sizeof(offset))};

        // auto [_block_id, _block_size, _block_num_links] =
        //     read_header_section<t_logger>(p_file, p_logger, _link);
        if (_offset == 0) {
          continue;
        }

        auto _block_id{read_block<t_logger>(
            p_file, p_logger, _offset,
            mem::block_ref{t_block_id, p_offset_start}, p_level + 1)};

        mem::block_ref _block_id_link{_block_id, _offset};

        _block.add_link(std::move(_block_id_link));
      }
    }

    _ptr = reinterpret_cast<const std::uint8_t *>(
        &_buf[p_num_links * sizeof(mem::block_link)]);

    _block.set_data_section(std::move(mem::data_section_t<t_block_id>{_ptr}));

    TNCT_LOG_INF(p_logger, format::fmt(std::string(p_level, '\t'), _block));
  }
};

// template <mem::block_id t_block_id> using read_block_t =
// std::function<void()>;

template <tnct::traits::log::logger t_logger>
mem::block_id
read_block(std::ifstream &p_file, t_logger &p_logger, offset p_offset_start,
           std::optional<mem::block_ref> p_parent = std::nullopt,
           std::size_t p_level = 0) {
  if (!p_file.seekg(p_offset_start).good()) {
    log_and_throw(p_logger,
                  format::fmt("error seekg for ", p_offset_start, " bytes"));
  }

  auto [_block_id, _block_size, _block_num_links] =
      read_header_section<t_logger>(p_file, p_logger, p_offset_start);

  // TNCT_LOG_INF(p_logger,
  //              format::fmt("block ",
  //              mem::block_id_converter::to_str(_block_id),
  //                          " has ", _block_size, " bytes and ",
  //                          _block_num_links, " links"));

  switch (_block_id) {
  case mem::block_id::HD:
    block_reader_t<mem::block_id::HD>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::MD:
    block_reader_t<mem::block_id::MD>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::TX:
    block_reader_t<mem::block_id::TX>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::FH:
    block_reader_t<mem::block_id::FH>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::AT:
    block_reader_t<mem::block_id::AT>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::EV:
    block_reader_t<mem::block_id::EV>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::DG:
    block_reader_t<mem::block_id::DG>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::CG:
    block_reader_t<mem::block_id::CG>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::SI:
    block_reader_t<mem::block_id::SI>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::CN:
    block_reader_t<mem::block_id::CN>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::CC:
    block_reader_t<mem::block_id::CC>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::CA:
    block_reader_t<mem::block_id::CA>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::DT:
    block_reader_t<mem::block_id::DT>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::SR:
    block_reader_t<mem::block_id::SR>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::RD:
    block_reader_t<mem::block_id::RD>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::SD:
    block_reader_t<mem::block_id::SD>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::DL:
    block_reader_t<mem::block_id::DL>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::DZ:
    block_reader_t<mem::block_id::DZ>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  case mem::block_id::HL:
    block_reader_t<mem::block_id::HL>()(
        p_file, p_logger, p_offset_start + header_section_size, _block_size,
        _block_num_links, p_parent, p_level);
    break;
  default:
    break;
  }

  return _block_id;
}

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

    read_block(_file, p_logger, _id_block_size);

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

// template <mem::block_type t_block_Id, tnct::traits::log::logger t_logger>
// struct read_link_section;

// template <tnct::traits::log::logger t_logger>

// struct read_link_section<mem::HD, t_logger> {
//   [[nodiscard]] std::pair<mem::link_section<t_logger>, std::streamsize>
//   operator()(std::ifstream &p_file, t_logger &p_logger,
//              std::streamsize p_offset_start) {
//     constexpr std::uint8_t _num_links{6};
//     constexpr std::uint8_t _buf_size{_num_links * sizeof(mem::block_link)};

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
//     _ptr += sizeof(mem::block_link);

//     _result.first.add_link(mem::FH,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::block_link);

//     _result.first.add_link(mem::CH,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::block_link);

//     _result.first.add_link(mem::AT,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::block_link);

//     _result.first.add_link(mem::EV,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::block_link);

//     _result.first.add_link(mem::TX,
//                            byte_array::from_little<std::uint64_t>(_ptr));
//     _ptr += sizeof(mem::block_link);

//     return _result;
//   }
// };

// template <> struct data_section_t<block_id::HD> {

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
