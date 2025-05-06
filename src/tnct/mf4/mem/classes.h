/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_MEM_CLASSES_H
#define TNCT_MF4_MEM_CLASSES_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>

#include "tnct/format/fmt.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::mem {

enum class Result : std::uint8_t { ok = 0, file_not_found = 1, error_reading };

std::ostream &operator<<(std::ostream &out, Result result) {
  switch (result) {
  case Result::ok:
    out << "OK";
    break;
  case Result::file_not_found:
    out << "file not found";
    break;
  case Result::error_reading:
    out << "error reading";
    break;
  default:
    out << "unknown";
    break;
  }
  return out;
}

using LINK = std::int64_t;

// static constexpr LINK invalid_link{0};

class Traits {
public:
  using Id = std::uint8_t;

  // Identification block. Identification of the file as MDF file
  static constexpr Id ID = 0;
  // Header block. General description of the measurement file
  static constexpr Id HD = 1;
  // Metadata block. Container for an XML string of variable length
  static constexpr Id MD = 2;
  // Text block. Container for a plain string of variable  length
  static constexpr Id TX = 3;
  // File history block. Change history information of the MDF file
  static constexpr Id FH = 4;
  // Channel hierarchy block. Definition of a logical structure/hierarchy for
  // channels
  static constexpr Id CH = 5;
  // Attachment block. Container for binary data or a reference to an external
  // file
  static constexpr Id AT = 6;
  // Event block. Description of an event
  static constexpr Id EV = 7;
  // Data group block. Description of data block that may refer to one or more
  // channel groups
  static constexpr Id DG = 8;
  // Channel group block. Description of a channel group; i.e. channels which
  // are always measured jointly
  static constexpr Id CG = 9;
  // Source information block. Specifies source information for a channel or for
  // the acquisition of a channel group.
  static constexpr Id SI = 10;
  // Channel block. Description of a channel; i.e. information about the
  // measured signal and how the signal nums are stored.
  static constexpr Id CN = 11;
  // Channel conversion block. Description of a conversion formula for a channel
  static constexpr Id CC = 12;
  // Channel array block. Description of an array  dependency (N-dimensional
  // matrix of channels with equal data type) including axes.
  static constexpr Id CA = 13;
  // Data block. Container for data records with signal nums
  static constexpr Id DT = 14;
  // Sample reduction block. Description of reduced/condensed signal nums which
  // can be used for preview
  static constexpr Id SR = 15;
  // Reduction data block. Container for data record triples with result of a
  // sample reduction
  static constexpr Id RD = 16;
  // Signal data block. Container for signal nums of variable length
  static constexpr Id SD = 17;
  // Data list block. Ordered list of links to (signal/reduction) data blocks if
  // the data is spread over multiple blocks
  static constexpr Id DL = 18;
  // Data zipped block. Container for zipped (compressed) data section of a
  // DT/SD/RD block as replacement of such a block.
  static constexpr Id DZ = 19;
  // Header of list block. Header information for linked list of data blocks
  static constexpr Id HL = 20;

private:
  static constexpr decltype(HL) num_ids{HL + 1};

public:
  using Str = char[5];

  using Strs = std::array<Str, num_ids>;
  using NumLinks = std::array<std::size_t, num_ids>;

  static constexpr Strs strs_{"##ID", "##HD", "##MD", "##TX", "##FH", "##CH",
                              "##AT", "##EV", "##DG", "##CG", "##SI", "##CN",
                              "##CC", "##CA", "##DT", "##SR", "##RD", "##SD",
                              "##DL", "##DZ", "##HL"};

  static constexpr NumLinks num_links_{
      0, // ID
      6, // HD
      0, // MD
      0, // TX
      0, // FH
      0, // CH
      0, // AT
      0, // EV
      0, // DG
      0, // CG
      0, // SI
      0, // CN
      0, // CC
      0, // CA
      0, // DT
      0, // SR
      0, // RD
      0, // SD
      0, // DL
      0, // DZ
      0, // HL
  };
};

// referencing and unique link?

template <Traits::Id TBlockId, traits::log::logger t_logger> struct Block;

template <traits::log::logger t_logger> struct Block<Traits::ID, t_logger>;

template <traits::log::logger t_logger> struct Block<Traits::HD, t_logger>;

template <traits::log::logger t_logger>
struct Block<Traits::ID, t_logger> final {
  Block() = delete;
  Block(const Block &) = delete;
  Block(Block &&p_block) : logger_(p_block.logger_) {
    std::strcpy(id_file_, p_block.GetIdFile());
    std::strcpy(id_vers_, p_block.GetIdVersionStr());
    std::strcpy(id_prog_, p_block.GetIdProg());
    id_ver_ = p_block.GetIdVersion();
    id_unfin_flags_ = p_block.GetIdUnfinFlags();
    id_custom_unfin_flags_ = p_block.GetIdCustomUnfinFlags();
  }
  ~Block() = default;

  Block(t_logger &p_logger, const char *id_file, const char *id_vers,
        const char *id_prog, std::uint16_t id_ver, std::uint16_t id_unfin_flags,
        std::uint16_t id_custom_unfin_flags)
      : logger_{p_logger} {
    bool error{false};
    if (std::strlen(id_file) != str_field_size) {
      TNCT_LOG_ERR(logger_,
                   format::fmt("id file should be ",
                               static_cast<std::uint16_t>(str_field_size),
                               " but it is ", std::strlen(id_file)));
      error = true;
    }
    std::strcpy(id_file_, id_file);

    if (std::strlen(id_vers) != str_field_size) {
      TNCT_LOG_ERR(logger_,
                   format::fmt("version id should be ",
                               static_cast<std::uint16_t>(str_field_size),
                               " but it is ", std::strlen(id_file)));
      error = true;
    }
    std::strcpy(id_vers_, id_vers);

    if (std::strlen(id_prog) != str_field_size) {
      TNCT_LOG_ERR(logger_,
                   format::fmt("program id should be ",
                               static_cast<std::uint16_t>(str_field_size),
                               " but it is ", std::strlen(id_prog)));
      error = true;
    }
    std::strcpy(id_prog_, id_prog);

    id_ver_ = id_ver;
    id_unfin_flags_ = id_unfin_flags;
    id_custom_unfin_flags_ = id_custom_unfin_flags;

    if (error) {
      throw std::runtime_error("ID block malformed");
    }
  }

  Block &operator=(const Block &) = delete;
  Block &operator=(Block &&) = delete;

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
std::ostream &operator<<(std::ostream &out,
                         const Block<Traits::ID, t_logger> &obj) {
  out << "Identification: file = '" << obj.GetIdFile() << "', version = ('"
      << obj.GetIdVersionStr() << "'," << obj.GetIdVersion() << ") program = '"
      << obj.GetIdProg() << "', id_unfin_flags = " << obj.GetIdUnfinFlags()
      << ", id_custom_unfin_flags = " << obj.GetIdCustomUnfinFlags()
      << ", is finalized? " << std::boolalpha << obj.IsFinalized();

  return out;
}

template <Traits::Id TBlockId, traits::log::logger t_logger>
struct HeaderSection {
  HeaderSection() = delete;
  HeaderSection(const HeaderSection &) = delete;
  HeaderSection(HeaderSection &&p_obj) : m_logger(p_obj.m_logger) {
    std::strcpy(m_id, p_obj.GetId());
    m_length = p_obj.GetLenght();
    m_link_count = p_obj.GetLinkCount();
  }
  HeaderSection &operator=(const HeaderSection &) = delete;
  HeaderSection &operator=(HeaderSection &&) = delete;
  HeaderSection(t_logger &p_logger, const char *p_id, uint64_t p_lenght,
                uint64_t p_link_count)
      : m_logger(p_logger) {
    if (std::strlen(p_id) > id_field_size) {
      TNCT_LOG_ERR(m_logger,
                   format::fmt("id should be ", id_field_size,
                               " long, but it is ", std::strlen(p_id),
                               " for block ", Traits::strs_[TBlockId]));
      throw std::runtime_error("error creating HeaderSection");
    }
    std::memset(m_id, '\0', id_field_size + 1);
    std::strcpy(m_id, p_id);

    m_length = p_lenght;
    m_link_count = p_link_count;
  }

  ~HeaderSection() = default;

  const char *GetId() const { return m_id; };
  std::uint64_t GetLenght() const { return m_length; }
  std::uint64_t GetLinkCount() const { return m_link_count; }

  static constexpr uint8_t id_field_size{4};

private:
  t_logger &m_logger;

  char m_id[id_field_size + 1];
  std::uint64_t m_length{std::numeric_limits<std::uint64_t>::max()};
  std::uint64_t m_link_count{std::numeric_limits<std::uint64_t>::max()};
};

template <Traits::Id TBlockId, traits::log::logger t_logger>
std::ostream &operator<<(std::ostream &out,
                         const HeaderSection<TBlockId, t_logger> &obj) {
  out << "Header Section id = '" << obj.GetId()
      << "', length = " << obj.GetLenght()
      << ", link count = " << obj.GetLinkCount();
  return out;
}

// template <Traits::Id TBlockId> struct LinkSection {
//   static constexpr std::streamsize size{static_cast<std::streamsize>(
//       Traits::num_links_[TBlockId] * sizeof(LINK))};

//   using Links = std::array<LINK, Traits::num_links_[TBlockId]>;

//   LinkSection() = delete;
//   LinkSection(const LinkSection &) = delete;
//   LinkSection(LinkSection &&) = delete;

//   LinkSection(std::streamsize offset_start, std::ifstream &file) {

//     for (typename Links::size_type i = 0; i < std::tuple_size_v<Links>; ++i)
//     {
//       std::uint64_t tmp{std::numeric_limits<std::uint64_t>::max()};
//       if (!read_unsigned(file, offset_start + i * sizeof(LINK), tmp)) {
//         ctw::log::err(ctw::format::fmt("error reading LINK #", i,
//                                        ", from block ",
//                                        Traits::strs_[TBlockId]));
//         throw std::runtime_error("error reading LINK section");
//       }
//       links_[i] = static_cast<LINK>(tmp);
//     }
//   }

//   ~LinkSection() = default;

//   LinkSection &operator=(const LinkSection &) = delete;
//   LinkSection &operator=(LinkSection &&) = delete;

//   Links::size_type num_links() const { return links_.size(); }

//   std::optional<LINK> get(Links::size_type index) const {
//     if (index > links_.size()) {
//       return std::nullopt;
//     }
//     return {links_[index]};
//   }

// private:
//   Links links_;
// };

// template <Traits::Id TBlockId>
// std::ostream &operator<<(std::ostream &out, const LinkSection<TBlockId> &obj)
// {
//   out << "link section of " << Traits::strs_[TBlockId] << ": ";
//   for (typename LinkSection<TBlockId>::Links::size_type i = 0;
//        i < obj.num_links(); ++i) {
//     out << obj.get(i).value() << ' ';
//   }
//   return out;
// }

// template <Traits::Id TBlockId> struct DataSection;

// template <> struct DataSection<Traits::HD> {

//   static constexpr std::streamsize size{
//       sizeof(std::uint64_t) + sizeof(std::int16_t) + sizeof(std::int16_t) +
//       sizeof(std::uint8_t) + sizeof(std::uint8_t) + sizeof(std::uint8_t) +
//       sizeof(std::uint8_t) + sizeof(double) + sizeof(double)};

//   DataSection(std::streamsize offset_start, std::ifstream &file) {
//     char buf[size];
//     std::memset(&buf[0], 0, size);

//     file.seekg(offset_start);
//     if (!file.read(&buf[0], size).good()) {
//       ctw::log::err(ctw::format::fmt("error reading data section for ",
//                                      Traits::strs_[Traits::HD]));
//       throw std::runtime_error("error reading data section");
//     }

//     auto ptr{reinterpret_cast<const std::uint8_t *>(&buf[0])};

//     hd_start_time_ns_ =
//         ctw::byte_array::from_little<decltype(hd_start_time_ns_)>(ptr);
//     ptr += sizeof(hd_start_time_ns_);

//     hd_tz_offset_min_ =
//         ctw::byte_array::from_little<decltype(hd_start_time_ns_)>(ptr);
//     ptr += sizeof(hd_tz_offset_min_);

//     hd_dst_offset_min_ =
//         ctw::byte_array::from_little<decltype(hd_dst_offset_min_)>(ptr);
//     ptr += sizeof(hd_dst_offset_min_);

//     hd_time_flags_ =
//         ctw::byte_array::from_little<decltype(hd_time_flags_)>(ptr);
//     ptr += sizeof(hd_time_flags_);

//     hd_flags_ = ctw::byte_array::from_little<decltype(hd_flags_)>(ptr);
//     ptr += sizeof(hd_flags_) + sizeof(hd_reserved_);

//     hd_start_angle_rad_ =
//         ctw::byte_array::from_little<decltype(hd_start_angle_rad_)>(ptr);
//     ptr += sizeof(hd_start_angle_rad_);

//     hd_start_distance_m_ =
//         ctw::byte_array::from_little<decltype(hd_start_distance_m_)>(ptr);
//   }

//   std::uint64_t Gethd_start_time_ns() const { return hd_start_time_ns_; }
//   std::int16_t Gethd_tz_offset_min() const { return hd_tz_offset_min_; }
//   std::int16_t Gethd_dst_offset_min() const { return hd_dst_offset_min_; }
//   std::uint8_t Gethd_time_flags() const { return hd_time_flags_; }
//   std::uint8_t Gethd_time_class() const { return hd_time_class_; }
//   std::uint8_t Gethd_flags() const { return hd_flags_; };
//   double Gethd_start_angle_rad() const { return hd_start_angle_rad_; }
//   double Gethd_start_distance_m() const { return hd_start_distance_m_; }

// private:
//   std::uint64_t hd_start_time_ns_;
//   std::int16_t hd_tz_offset_min_;
//   std::int16_t hd_dst_offset_min_;
//   std::uint8_t hd_time_flags_;
//   std::uint8_t hd_time_class_;
//   std::uint8_t hd_flags_;
//   std::uint8_t hd_reserved_;
//   double hd_start_angle_rad_;
//   double hd_start_distance_m_;
// };

// std::ostream &operator<<(std::ostream &out,
//                          const DataSection<Traits::HD> &obj) {
//   out << "data section of " << Traits::strs_[Traits::HD] << ": "
//       << obj.Gethd_start_time_ns() << " " << obj.Gethd_tz_offset_min() << ' '
//       << obj.Gethd_dst_offset_min() << ' '
//       << static_cast<uint16_t>(obj.Gethd_time_flags()) << ' '
//       << static_cast<uint16_t>(obj.Gethd_time_class()) << ' '
//       << static_cast<uint16_t>(obj.Gethd_flags()) << ' '
//       << obj.Gethd_start_angle_rad() << ' ' << obj.Gethd_start_distance_m();
//   return out;
// }

} // namespace tnct::mf4::mem

#endif
