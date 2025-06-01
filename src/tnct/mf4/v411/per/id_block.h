/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_ID_BLOCK_H
#define TNCT_MF4_PER_ID_BLOCK_H

#include <fstream>

#include "tnct/byte_array/classes.h"
#include "tnct/generic/log_and_throw.h"
#include "tnct/mf4/v411/mem/id_block.h"
#include "tnct/mf4/v411/per/basic_types.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::per {

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
    generic::log_and_throw(p_logger, "could not seekg to 0");
  }
  if (!file.read(&buf[0], _amount_to_read).good()) {
    generic::log_and_throw(p_logger, "error reading id block");
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

} // namespace tnct::mf4::v411::per

#endif
