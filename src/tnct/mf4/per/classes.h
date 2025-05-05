/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_CLASSES_H
#define TNCT_MF4_PER_CLASSES_H

#include <fstream>

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
tnct::mf4::mem::Block<tnct::mf4::mem::Traits::ID, t_logger>
read_id_block(std::ifstream &file, t_logger &p_logger) {
  constexpr std::streamsize size{64};
  using IdReserverd2 = std::array<std::uint8_t, 30>;
  static constexpr uint8_t str_field_size{8};
  char id_file_[str_field_size + 1];
  std::memset(id_file_, '\0', str_field_size + 1);
  char id_vers_[str_field_size + 1];
  std::memset(id_vers_, '\0', str_field_size + 1);
  char id_prog_[str_field_size + 1];
  std::memset(id_prog_, '\0', str_field_size + 1);
  std::uint32_t id_reserved1_{0x00};
  std::uint16_t id_ver_{0xFFFF};

  std::uint16_t id_unfin_flags_{0XFFFF};
  std::uint16_t id_custom_unfin_flags_{0xFFFF};

  char buf[size];
  std::memset(&buf[0], 0, size);

  file.seekg(0);
  if (!file.read(&buf[0], size).good()) {
    TNCT_LOG_ERR(
        p_logger,
        tnct::format::fmt(
            "error reading data section for ",
            tnct::mf4::mem::Traits::strs_[tnct::mf4::mem::Traits::HD]));
    throw std::runtime_error("error reading data section");
  }

  std::memcpy(&id_file_[0], &buf[0], str_field_size);
  std::memcpy(&id_vers_[0], &buf[str_field_size], str_field_size);
  std::memcpy(&id_prog_[0], &buf[2 * str_field_size], str_field_size);

  auto ptr{reinterpret_cast<const std::uint8_t *>(
      &buf[3 * str_field_size + sizeof(id_reserved1_)])};

  id_ver_ = tnct::byte_array::from_little<decltype(id_ver_)>(ptr);
  ptr += sizeof(id_ver_) + std::tuple_size_v<IdReserverd2>;

  id_unfin_flags_ =
      tnct::byte_array::from_little<decltype(id_unfin_flags_)>(ptr);
  ptr += sizeof(id_unfin_flags_);

  id_custom_unfin_flags_ =
      tnct::byte_array::from_little<decltype(id_custom_unfin_flags_)>(ptr);

  return {p_logger,
          id_file_,
          id_vers_,
          id_prog_,
          id_ver_,
          id_unfin_flags_,
          id_custom_unfin_flags_};
}

template <tnct::traits::log::logger t_logger>
mem::Result mf4_reader(std::string_view file_name, t_logger &p_logger) {
  try {
    std::ifstream file(file_name.data(), std::ios_base::binary);
    if (!file.good()) {
      return mf4::mem::Result::file_not_found;
    }

    mem::Block<tnct::mf4::mem::Traits::ID, t_logger> id_block{
        read_id_block(file, p_logger)};
    TNCT_LOG_INF(p_logger, tnct::format::fmt(id_block));

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
  } catch (std::exception &ex) {
    std::cout << "ERROR: " << ex.what() << std::endl;
  } catch (...) {
    std::cout << "ERROR unknow" << std::endl;
  }
  return mem::Result::ok;
}

} // namespace tnct::mf4::per

#endif
