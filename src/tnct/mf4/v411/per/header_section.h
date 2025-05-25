/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_HEADE_SECTION_H
#define TNCT_MF4_PER_HEADE_SECTION_H

#include <fstream>

#include "tnct/byte_array/classes.h"
#include "tnct/format/fmt.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/per/basic_types.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::per {

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

} // namespace tnct::mf4::v411::per

#endif
