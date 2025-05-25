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
#include "tnct/mf4/v411/log_and_throw.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/block_ref.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_hd.h"
#include "tnct/mf4/v411/mem/file.h"
#include "tnct/mf4/v411/mem/id_block.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::per {

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

template <mem::block_id t_block_id> struct data_section_reader_t {
  mem::data_section_t<t_block_id> operator()(const std::uint8_t *) {
    return {};
  }
};

template <> struct data_section_reader_t<mem::block_id::HD> {
  mem::data_section_t<mem::block_id::HD> operator()(const std::uint8_t *p_buf) {

    using hd_start_time_ns = std::uint64_t;
    using hd_tz_offset_min = std::int16_t;
    using hd_dst_offset_min = std::int16_t;
    using hd_time_flags = std::uint8_t;
    using hd_time_class = std::uint8_t;
    using hd_flags = std::uint8_t;
    using reserved = std::uint8_t;
    using hd_start_angle_rad = double;
    using hd_start_angle_m = double;

    static constexpr std::uint8_t size_hd_start_time_ns{
        sizeof(hd_start_time_ns)};
    static constexpr std::uint8_t size_hd_tz_offset_min{
        sizeof(hd_tz_offset_min)};
    static constexpr std::uint8_t size_hd_dst_offset_min{
        sizeof(hd_dst_offset_min)};
    static constexpr std::uint8_t size_hd_time_flags{sizeof(hd_time_flags)};
    static constexpr std::uint8_t size_hd_time_class{sizeof(hd_time_class)};
    static constexpr std::uint8_t size_hd_flags{sizeof(hd_flags)};
    static constexpr std::uint8_t size_reserved{sizeof(reserved)};
    static constexpr std::uint8_t size_hd_start_angle_rad{
        sizeof(hd_start_angle_rad)};

    return {
        byte_array::from_little<hd_start_time_ns>(p_buf),

        byte_array::from_little<hd_tz_offset_min>(p_buf +
                                                  size_hd_start_time_ns),

        byte_array::from_little<hd_dst_offset_min>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min),

        byte_array::from_little<hd_time_flags>(p_buf + size_hd_start_time_ns +
                                               size_hd_tz_offset_min +
                                               size_hd_dst_offset_min),

        byte_array::from_little<hd_time_class>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags),

        byte_array::from_little<hd_flags>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags + size_hd_time_class),

        byte_array::from_little<hd_start_angle_rad>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags + size_hd_time_class +
            size_hd_flags + size_reserved),

        byte_array::from_little<hd_start_angle_m>(
            p_buf + size_hd_start_time_ns + size_hd_tz_offset_min +
            size_hd_dst_offset_min + size_hd_time_flags + size_hd_time_class +
            size_hd_flags + size_reserved + size_hd_start_angle_rad)};
  }
};

template <mem::block_id t_block_id> struct block_reader_t {
  template <tnct::traits::log::logger t_logger>
  mem::block_ref operator()(std::ifstream &p_file, t_logger &p_logger,
                            int64_t p_offset_start, amount_to_read p_block_size,
                            std::size_t p_num_links, mem::file &p_mf4_file,
                            std::optional<mem::block_ref> p_parent,
                            std::size_t p_level = 0) {

    const amount_to_read size_to_read{p_block_size - header_section_size};

    const offset _start{p_offset_start +
                        static_cast<offset>(header_section_size)};

    if (!p_file.seekg(_start).good()) {
      log_and_throw(p_logger,
                    format::fmt("error seekg for ", _start, " bytes"));
    }

    std::vector<char> _buf(size_to_read, 0);
    if (!p_file.read(_buf.data(), size_to_read).good()) {
      log_and_throw(p_logger, format::fmt("error reading ", size_to_read,
                                          " bytes for header block "));
    }

    mem::block_index _block_index{
        p_mf4_file.add<t_block_id>(p_parent, p_logger)};

    mem::block_ref _block_ref{t_block_id, _block_index};

    auto _ptr = reinterpret_cast<const std::uint8_t *>(&_buf[0]);
    {
      for (decltype(p_num_links) _i = 0; _i < p_num_links; ++_i) {

        offset _offset{
            byte_array::from_little<offset>(_ptr + _i * sizeof(offset))};

        if (_offset == 0) {
          continue;
        }

        mem::block_ref _inner_block_ref{read_block<t_logger>(
            p_file, p_logger, _offset, p_mf4_file, {_block_ref}, p_level + 1)};

        p_mf4_file.get<t_block_id>(_block_index, p_logger)
            .add_link(std::move(_inner_block_ref));
      }
    }

    _ptr += (p_num_links * sizeof(mem::block_index));

    data_section_reader_t<t_block_id> _data_section_reader;

    auto &_block{p_mf4_file.get<t_block_id>(_block_index, p_logger)};
    _block.set_data_section(std::move(_data_section_reader(_ptr)));

    // TNCT_LOG_INF(p_logger, format::fmt(std::string(p_level, '\t'), _block));

    return _block_ref;
  }
};

template <tnct::traits::log::logger t_logger>
mem::block_ref read_block(std::ifstream &p_file, t_logger &p_logger,
                          offset p_offset_start, mem::file &p_mf4_file,
                          std::optional<mem::block_ref> p_parent = std::nullopt,
                          std::size_t p_level = 0) {
  if (!p_file.seekg(p_offset_start).good()) {
    log_and_throw(p_logger,
                  format::fmt("error seekg for ", p_offset_start, " bytes"));
  }

  auto [_block_id, _block_size, _block_num_links] =
      read_header_section<t_logger>(p_file, p_logger, p_offset_start);

  mem::block_ref _block_ref;

  auto _block_reader = [&]<mem::block_id t_block_id>() {
    _block_ref = block_reader_t<t_block_id>()(p_file, p_logger, p_offset_start,
                                              _block_size, _block_num_links,
                                              p_mf4_file, p_parent, p_level);
  };

  mem::visit_blocks_ids(_block_id, _block_reader);

  return _block_ref;
}

template <tnct::traits::log::logger t_logger>
void mf4_reader(std::string_view file_name, t_logger &p_logger,
                mem::file &p_mf4_file) {

  std::ifstream _file(file_name.data(), std::ios_base::binary);
  if (!_file.good()) {
    log_and_throw(p_logger, format::fmt("file ", file_name.data(),
                                        " could not be opened"));
  }

  try {
    auto [_id_block, _id_block_size] = read_id_block(_file, p_logger);
    TNCT_LOG_INF(p_logger,
                 format::fmt(_id_block, " - size = ", _id_block_size));

    read_block(_file, p_logger, _id_block_size, p_mf4_file);

  } catch (std::exception &_ex) {
    log_and_throw(p_logger, _ex.what());
  } catch (...) {
    log_and_throw(p_logger, "ERROR unknow");
  }
}

} // namespace tnct::mf4::v411::per

#endif
