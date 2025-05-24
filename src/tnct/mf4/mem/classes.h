/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_MEM_CLASSES_H
#define TNCT_MF4_MEM_CLASSES_H

#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <vector>

#include "tnct/byte_array/classes.h"
#include "tnct/format/fmt.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::mem {

using block_link = std::int64_t;

enum class result : std::uint8_t { ok = 0, file_not_found = 1, error_reading };

std::ostream &operator<<(std::ostream &out, result result) {
  switch (result) {
  case result::ok:
    out << "OK";
    break;
  case result::file_not_found:
    break;
  case result::error_reading:
    out << "error reading";
    break;
  default:
    out << "unknown";
    break;
  }
  return out;
}

enum class block_id : std::uint8_t {

  // Identification block. Identification of the file as MDF file
  ID = 0,
  // Header block. General description of the measurement file
  HD = 1,
  // Metadata block. Container for an XML string of variable length
  MD = 2,
  // Text block. Container for a plain string of variable  length
  TX = 3,
  // File history block. Change history information of the MDF file
  FH = 4,
  // Channel hierarchy block. Definition of a logical structure/hierarchy for
  // channels
  CH = 5,
  // Attachment block. Container for binary data or a reference to an external
  // file
  AT = 6,
  // Event block. Description of an event
  EV = 7,
  // Data group block. Description of data block that may refer to one or more
  // channel groups
  DG = 8,
  // Channel group block. Description of a channel group, i.e. channels which
  // are always measured jointly
  CG = 9,
  // Source information block. Specifies source information for a channel or
  // for the acquisition of a channel group.
  SI = 10,
  // Channel block. Description of a channel, i.e. information about the
  // measured signal and how the signal nums are stored.
  CN = 11,
  // Channel conversion block. Description of a conversion formula for a
  // channel
  CC = 12,
  // Channel array block. Description of an array  dependency (N-dimensional
  // matrix of channels with equal data type) including axes.
  CA = 13,
  // Data block. Container for data records with signal nums
  DT = 14,
  // Sample reduction block. Description of reduced/condensed signal nums
  // which can be used for preview
  SR = 15,
  // Reduction data block. Container for data record triples with result of a
  // sample reduction
  RD = 16,
  // Signal data block. Container for signal nums of variable length
  SD = 17,
  // Data list block. Ordered list of links to (signal/reduction) data blocks
  // if the data is spread over multiple blocks
  DL = 18,
  // Data zipped block. Container for zipped (compressed) data section of a
  // DT/SD/RD block as replacement of such a block.
  DZ = 19,
  // Header of list block. Header information for linked list of data blocks
  HL = 20,

};

using block_id_str = char[5];

struct block_id_converter {
  static const block_id_str &to_str(block_id p_id) {
    return m_array[static_cast<block_id_type>(p_id)];
  }

  static std::optional<block_id> to_id(const block_id_str p_id_str) {
    const auto _ite{std::find_if(m_array.begin(), m_array.end(),
                                 [&](const block_id_str &item) {
                                   return (std::strcmp(p_id_str, item) == 0);
                                 })};
    if (_ite == m_array.end()) {
      return std::nullopt;
    }
    const auto _distance{std::distance(m_array.begin(), _ite)};
    const block_id _block_id{static_cast<block_id_type>(_distance)};
    return {_block_id};
  }

private:
  using array =
      std::array<block_id_str,
                 static_cast<std::underlying_type_t<block_id>>(
                     block_id::HL) +
                     1>;

  using block_id_type = std::underlying_type_t<block_id>;

  static constexpr array m_array{"##ID", "##HD", "##MD", "##TX", "##FH", "##CH",
                                 "##AT", "##EV", "##DG", "##CG", "##SI", "##CN",
                                 "##CC", "##CA", "##DT", "##SR", "##RD", "##SD",
                                 "##DL", "##DZ", "##HL"};
};

struct block_ref {
  block_ref() = delete;

  block_ref(block_id p_block_id, block_link p_block_link)
      : m_block_id(p_block_id), m_block_link(p_block_link) {}

  block_ref(const block_ref &) = default;

  block_ref(block_ref &&) = default;

  ~block_ref() = default;

  block_ref &operator=(const block_ref &) = default;

  block_ref &operator=(block_ref &&) = default;

  constexpr bool operator==(const block_ref &p_obj) const {
    return (m_block_id == p_obj.m_block_id) &&
           (m_block_link == p_obj.m_block_link);
  }

  constexpr bool operator!=(const block_ref &p_obj) const {
    return !(*this == p_obj);
  }

  constexpr bool operator<(const block_ref &p_obj) const {
    if (m_block_id < p_obj.m_block_id) {
      return true;
    }
    if (m_block_id > p_obj.m_block_id) {
      return false;
    }

    if (m_block_link < p_obj.m_block_link) {
      return true;
    }

    return false;
  }

  block_id id() const { return m_block_id; }

  block_link link() const { return m_block_link; }

private:
  block_id m_block_id;
  block_link m_block_link;
};

std::ostream &operator<<(std::ostream &p_out, const block_ref &p_obj) {
  p_out << "id = " << block_id_converter::to_str(p_obj.id())
        << ", link = " << p_obj.link();
  return p_out;
}

template <block_id t_block_Id> struct data_section_t;

template <> struct data_section_t<block_id::MD> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::TX> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::FH> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::AT> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::EV> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::DG> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::CG> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::SI> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::CN> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::CC> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::CA> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::DT> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::SR> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::RD> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::SD> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::DL> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::DZ> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <> struct data_section_t<block_id::HL> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *) {}

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  ~data_section_t() = default;
};

template <block_id t_block_Id>
std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<t_block_Id> &) {
  p_out << " data section of " << block_id_converter::to_str(t_block_Id);
  return p_out;
}

template <> struct data_section_t<block_id::HD> final {
  data_section_t() = default;
  data_section_t(const std::uint8_t *p_buf) {
    std::uint64_t m_hd_start_time_ns =
        byte_array::from_little<decltype(m_hd_start_time_ns)>(p_buf);
    p_buf += sizeof(m_hd_start_time_ns);

    std::int16_t m_hd_tz_offset_min =
        byte_array::from_little<decltype(m_hd_start_time_ns)>(p_buf);
    p_buf += sizeof(m_hd_tz_offset_min);

    std::int16_t m_hd_dst_offset_min =
        byte_array::from_little<decltype(m_hd_dst_offset_min)>(p_buf);
    p_buf += sizeof(m_hd_dst_offset_min);

    std::uint8_t m_hd_time_flags =
        byte_array::from_little<decltype(m_hd_time_flags)>(p_buf);
    p_buf += sizeof(m_hd_time_flags);

    std::uint8_t m_hd_time_class =
        byte_array::from_little<decltype(m_hd_time_class)>(p_buf);
    p_buf += sizeof(m_hd_time_class);

    std::uint8_t m_hd_flags =
        byte_array::from_little<decltype(m_hd_flags)>(p_buf);
    p_buf += sizeof(m_hd_flags) + sizeof(std::uint8_t);

    double m_hd_start_angle_rad =
        byte_array::from_little<decltype(m_hd_start_angle_rad)>(p_buf);
    p_buf += sizeof(m_hd_start_angle_rad);

    double m_hd_start_distance_m =
        byte_array::from_little<decltype(m_hd_start_distance_m)>(p_buf);
  }

  data_section_t(const data_section_t &) = default;
  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;
  data_section_t &operator=(data_section_t &&) = default;

  std::uint64_t get_hd_start_time_ns() const { return m_hd_start_time_ns; }
  std::int16_t get_hd_tz_offset_min() const { return m_hd_tz_offset_min; }
  std::int16_t get_hd_dst_offset_min() const { return m_hd_dst_offset_min; }
  std::uint8_t get_hd_time_flags() const { return m_hd_time_flags; }
  std::uint8_t get_hd_time_class() const { return m_hd_time_class; }
  std::uint8_t get_hd_flags() const { return m_hd_flags; };
  double get_hd_start_angle_rad() const { return m_hd_start_angle_rad; }
  double get_hd_start_distance_m() const { return m_hd_start_distance_m; }

private:
  std::uint64_t m_hd_start_time_ns;
  std::int16_t m_hd_tz_offset_min;
  std::int16_t m_hd_dst_offset_min;
  std::uint8_t m_hd_time_flags;
  std::uint8_t m_hd_time_class;
  std::uint8_t m_hd_flags;
  double m_hd_start_angle_rad;
  double m_hd_start_distance_m;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::HD> &p_obj) {
  p_out << block_id_converter::to_str(block_id::HD) << ": "
        << p_obj.get_hd_start_time_ns() << " " << p_obj.get_hd_tz_offset_min()
        << ' ' << p_obj.get_hd_dst_offset_min() << ' '
        << static_cast<uint16_t>(p_obj.get_hd_time_flags()) << ' '
        << static_cast<uint16_t>(p_obj.get_hd_time_class()) << ' '
        << static_cast<uint16_t>(p_obj.get_hd_flags()) << ' '
        << p_obj.get_hd_start_angle_rad() << ' '
        << p_obj.get_hd_start_distance_m();
  return p_out;
}

template <block_id t_block_id> struct block_t final {

  using block_ids_links = std::vector<block_ref>;

  using const_links_iterator = block_ids_links::const_iterator;

  using data_section = data_section_t<t_block_id>;

  block_t() = delete;

  block_t(block_link p_position,
          std::optional<block_ref> p_parent = std::nullopt)
      : m_position(p_position), m_parent(p_parent) {}

  block_t(const block_t &) = delete;

  block_t(block_t &&) noexcept = default;

  ~block_t() = default;

  block_t &operator=(const block_t &) = delete;
  block_t &operator=(block_t &&) noexcept = default;

  constexpr bool operator==(const block_t &p_block) const {
    return (m_id == p_block.m_id) && (m_position == p_block.m_position);
  }

  constexpr bool operator!=(const block_t &p_block) const {
    return !(*this == p_block);
  }

  constexpr bool operator<(const block_t &p_block) const {
    if (m_id < p_block.m_id) {
      return true;
    }
    if (m_id > p_block.m_id) {
      return false;
    }
    if (m_position < p_block.m_position) {
      return true;
    }
    return false;
  }

  const block_id &get_id() const { return m_id; }

  const block_link &get_position() const { return m_position; }

  std::optional<block_ref> get_parent() const { return m_parent; }

  const_links_iterator begin() const { return m_block_ids_links.begin(); }

  const_links_iterator end() const { return m_block_ids_links.end(); }

  std::size_t get_num_links() const { return m_block_ids_links.size(); }

  void add_link(block_ref &&p_link) {
    m_block_ids_links.push_back(std::move(p_link));
  }

  void set_data_section(data_section &&p_data_section) {
    m_data_section = std::move(p_data_section);
  }

  const data_section &get_data_section() const { return m_data_section; }

private:
  block_id m_id{t_block_id};

  block_link m_position;

  std::optional<block_ref> m_parent;

  block_ids_links m_block_ids_links;

  data_section m_data_section;
};

// referencing and unique link?

template <block_id t_block_id>
std::ostream &operator<<(std::ostream &p_out,
                         const block_t<t_block_id> &p_block) {
  p_out << "{id = " << block_id_converter::to_str(p_block.get_id())
        << ", position = " << p_block.get_position() << ", parent {";
  if (p_block.get_parent().has_value()) {
    p_out << p_block.get_parent().value();
  } else {
    p_out << "NULL";
  }
  p_out << "}, links {";
  for (typename block_t<t_block_id>::const_links_iterator _ite =
           p_block.begin();
       _ite != p_block.end(); ++_ite) {
    p_out << *_ite << " - ";
  }
  p_out << "} ";
  p_out << ", data {" << p_block.get_data_section() << "} }";
  return p_out;
}

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

} // namespace tnct::mf4::mem

#endif
