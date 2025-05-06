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

// template <std::unsigned_integral TValue>
// [[nodiscard]] bool read_unsigned(std::ifstream &file, std::streamsize offset,
//                                  TValue &value) {
//   file.seekg(offset);
//   if (!file.good()) {
//     return false;
//   }
//   char buf[sizeof(TValue)];
//   file.read(&buf[0], sizeof(TValue));
//   if (!file.good()) {
//     return false;
//   }
//   value = tnct::byte_array::from_little<TValue>(
//       reinterpret_cast<const std::uint8_t *>(&buf[0]));
//   return true;
// }

// template <std::signed_integral TValue>
// [[nodiscard]] bool read_signed(std::ifstream &file, std::streamsize offset,
//                                TValue &value) {
//   std::make_unsigned_t<TValue> tmp;
//   if (!read_unsigned(file, offset, tmp)) {
//     return false;
//   }
//   value = static_cast<TValue>(tmp);
//   return true;
// }

// bool read_double(std::ifstream &file, std::streamsize offset, double &value)
// {
//   int64_t tmp{0};
//   if (!read_signed(file, offset, tmp)) {
//     return false;
//   }
//   std::memcpy(&value, &tmp, sizeof(double));
//   return true;
// }

// [[nodiscard]] bool read_string(std::ifstream &file, std::streamsize offset,
//                                char *value, std::uint8_t size) {
//   file.seekg(offset);
//   if (!file.good()) {
//     return false;
//   }

//   std::memset(value, 0, size + 1);
//   file.read(value, size);
//   if (!file.good()) {
//     return false;
//   }
//   return true;
// }

template <tnct::traits::log::logger t_logger>
[[nodiscard]] std::pair<mem::Block<mem::Traits::ID, t_logger>, std::streamsize>
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

  constexpr std::streamsize _size{
      3 * _str_field_size + sizeof(id_reserved_1) + sizeof(decltype(_id_ver)) +
      std::tuple_size_v<id_reserverd_2> + sizeof(decltype(_id_unfin_flags)) +
      sizeof(_id_custom_unfin_flags)};

  // p_size = _size;

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
      mem::Block<mem::Traits::ID, t_logger>{p_logger, _id_file, _id_vers,
                                            _id_prog, _id_ver, _id_unfin_flags,
                                            _id_custom_unfin_flags},
      std::streamsize{_size});

  return _pair;
}

template <mem::Traits::Id TBlockId, tnct::traits::log::logger t_logger>
[[nodiscard]] std::pair<mem::HeaderSection<TBlockId, t_logger>, std::streamsize>
read_header_section(std::ifstream &p_file, t_logger &p_logger,
                    std::streamsize p_offset_start) {
  constexpr std::size_t _id_field_size{4 * sizeof(char)};
  constexpr std::size_t _reserved_field_size{sizeof(std::uint8_t) * 4};
  constexpr std::size_t _length_field_size{sizeof(uint64_t)};
  constexpr std::size_t _link_count_field_size{sizeof(uint64_t)};
  constexpr std::size_t _total_size{_id_field_size + _reserved_field_size +
                                    _length_field_size +
                                    _link_count_field_size};

  char buf[_total_size];

  if (!p_file.seekg(p_offset_start).good()) {
    TNCT_LOG_ERR(p_logger, format::fmt("error 'seekg' to ", p_offset_start,
                                       " for header section of block ",
                                       mem::Traits::strs_[TBlockId]));
    throw std::runtime_error("error reading header section");
  }

  if (!p_file.read(buf, _total_size).good()) {
    TNCT_LOG_ERR(p_logger, format::fmt("error reading to ", p_offset_start,
                                       " for header section of block ",
                                       mem::Traits::strs_[TBlockId]));
    throw std::runtime_error("error reading header section");
  }

  char _id[_id_field_size + 1];
  std::memset(_id, '\0', _id_field_size + 1);
  std::memcpy(_id, &buf[0], _id_field_size);

  auto _ptr{reinterpret_cast<const std::uint8_t *>(
      &buf[_id_field_size + _reserved_field_size])};

  uint64_t _lenght{byte_array::from_little<decltype(_lenght)>(_ptr)};
  _ptr += _length_field_size;

  uint64_t _link_count{byte_array::from_little<decltype(_link_count)>(_ptr)};

  auto _pair = std::make_pair(
      mem::HeaderSection<TBlockId, t_logger>{p_logger, _id, _lenght,
                                             _link_count},
      std::streamsize{_total_size});

  return _pair;
}

template <tnct::traits::log::logger t_logger>
mem::Result mf4_reader(std::string_view file_name, t_logger &p_logger) {
  try {
    std::ifstream _file(file_name.data(), std::ios_base::binary);
    if (!_file.good()) {
      return mem::Result::file_not_found;
    }

    auto [_id_block, _id_block_size] = read_id_block(_file, p_logger);
    TNCT_LOG_INF(p_logger,
                 format::fmt(_id_block, " - size = ", _id_block_size));

    auto [_header_section, _header_section_size] =
        read_header_section<mem::Traits::HD, t_logger>(_file, p_logger,
                                                       _id_block_size);
    TNCT_LOG_INF(p_logger, format::fmt(_header_section,
                                       " - size = ", _header_section_size));

    // tnct::mf4::mem::HeaderSection<tnct::mf4::mem::Traits::HD> header_section{
    //     tnct::mf4::mem::Block<tnct::mf4::mem::Traits::ID>::size, file};
    // TNCT_LOG_INF(p_logger, tnct::format::fmt(header_section));

    // tnct::mf4::mem::LinkSection<tnct::mf4::mem::Traits::HD> link_section(
    //     tnct::mf4::mem::Block<tnct::mf4::mem::Traits::ID>::size +
    //         tnct::mf4::mem::HeaderSection<tnct::mf4::mem::Traits::HD>::size,
    //     file);
    // TNCT_LOG_INF(p_logger, tnct::format::fmt(link_section));

    // tnct::mf4::mem::DataSection<tnct::mf4::mem::Traits::HD> data_section(
    //     tnct::mf4::mem::Block<tnct::mf4::mem::Traits::ID>::size +
    //         tnct::mf4::mem::HeaderSection<tnct::mf4::mem::Traits::HD>::size +
    //         tnct::mf4::mem::LinkSection<tnct::mf4::mem::Traits::HD>::size,
    //     file);
    // TNCT_LOG_INF(p_logger, tnct::format::fmt(data_section));
    return mem::Result::ok;
  } catch (std::exception &ex) {
    std::cout << "ERROR: " << ex.what() << std::endl;
  } catch (...) {
    std::cout << "ERROR unknow" << std::endl;
  }
  return mem::Result::error_reading;
}

} // namespace tnct::mf4::per

#endif
