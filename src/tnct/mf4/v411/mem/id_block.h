/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_CLASSES_ID_BLOCK_H
#define TNCT_MF4_V411_MEM_CLASSES_ID_BLOCK_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "tnct/format/fmt.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <traits::log::logger t_logger> struct id_block final {
  id_block() = delete;
  id_block(const id_block &) = delete;
  id_block(id_block &&p_block) noexcept : logger_(p_block.logger_) {
    std::strncpy(id_file_, p_block.GetIdFile(), sizeof(id_file_));
    std::strncpy(id_vers_, p_block.GetIdVersionStr(), sizeof(id_file_));
    std::strncpy(id_prog_, p_block.GetIdProg(), sizeof(id_file_));
    id_ver_ = p_block.GetIdVersion();
    id_unfin_flags_ = p_block.GetIdUnfinFlags();
    id_custom_unfin_flags_ = p_block.GetIdCustomUnfinFlags();
  }
  ~id_block() = default;

  id_block(t_logger &p_logger, const char *id_file, const char *id_vers,
           const char *id_prog, std::uint16_t id_ver,
           std::uint16_t id_unfin_flags, std::uint16_t id_custom_unfin_flags)
      : logger_{p_logger} {
    bool error{false};
    if (std::strlen(id_file) != str_field_size) {
      TNCT_LOG_ERR(logger_,
                   format::fmt("id file should be ",
                               static_cast<std::uint16_t>(str_field_size),
                               " but it is ", std::strlen(id_file)));
      error = true;
    }
    std::strncpy(id_file_, id_file, sizeof(id_file_));

    if (std::strlen(id_vers) != str_field_size) {
      TNCT_LOG_ERR(logger_,
                   format::fmt("version id should be ",
                               static_cast<std::uint16_t>(str_field_size),
                               " but it is ", std::strlen(id_file)));
      error = true;
    }
    std::strncpy(id_vers_, id_vers, sizeof(id_vers_));

    if (std::strlen(id_prog) != str_field_size) {
      TNCT_LOG_ERR(logger_,
                   format::fmt("program id should be ",
                               static_cast<std::uint16_t>(str_field_size),
                               " but it is ", std::strlen(id_prog)));
      error = true;
    }
    std::strncpy(id_prog_, id_prog, sizeof(id_prog_));

    id_ver_ = id_ver;
    id_unfin_flags_ = id_unfin_flags;
    id_custom_unfin_flags_ = id_custom_unfin_flags;

    if (error) {
      throw std::runtime_error("ID block malformed");
    }
  }

  id_block &operator=(const id_block &) = delete;
  id_block &operator=(id_block &&) = delete;

  static constexpr std::streamsize size{64};

  const char *GetIdFile() const { return id_file_; }
  const char *GetIdVersionStr() const { return id_vers_; }
  const char *GetIdProg() const { return id_prog_; }
  std::uint16_t GetIdVersion() const { return id_ver_; }
  std::uint16_t GetIdUnfinFlags() const { return id_unfin_flags_; }
  std::uint16_t GetIdCustomUnfinFlags() const { return id_custom_unfin_flags_; }
  bool IsFinalized() const { return !std::strcmp(id_file_, "MDF     "); }

private:
  t_logger &logger_;
  static constexpr uint8_t str_field_size{8};
  char id_file_[str_field_size + 1];
  char id_vers_[str_field_size + 1];
  char id_prog_[str_field_size + 1];
  std::uint16_t id_ver_{0xFFFF};
  std::uint16_t id_unfin_flags_{0XFFFF};
  std::uint16_t id_custom_unfin_flags_{0xFFFF};
};

template <traits::log::logger t_logger>
std::ostream &operator<<(std::ostream &out, const id_block<t_logger> &obj) {
  out << "Identification: file = '" << obj.GetIdFile() << "', version = ('"
      << obj.GetIdVersionStr() << "'," << obj.GetIdVersion() << ") program = '"
      << obj.GetIdProg() << "', id_unfin_flags = " << obj.GetIdUnfinFlags()
      << ", id_custom_unfin_flags = " << obj.GetIdCustomUnfinFlags()
      << ", is finalized? " << std::boolalpha << obj.IsFinalized();

  return out;
}

} // namespace tnct::mf4::v411::mem

#endif
