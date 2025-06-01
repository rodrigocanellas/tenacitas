/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_EV_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_EV_H

#include <iostream>

#include "tnct/byte_array/classes.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::mem {

template <> struct data_section_t<block_id::EV> final {
  using ev_type = std::uint8_t;
  using ev_sync_type = std::uint8_t;
  using ev_range_type = std::uint8_t;
  using ev_cause = std::uint8_t;
  using ev_flags = std::uint8_t;
  using ev_scope_count = std::uint32_t;
  using ev_attachment_count = std::uint16_t;
  using ev_creator_index = std::uint16_t;
  using ev_sync_base_value = std::int64_t;
  using ev_sync_factor = double;

  data_section_t() = default;

  template <traits::log::logger t_logger>
  data_section_t(t_logger &p_logger, const std::uint8_t *p_buf,
                 std::size_t p_size) {
    byte_array::from_buffer_little_endian(
        p_logger, p_buf, p_size, std::source_location::current(), m_ev_type,
        m_ev_sync_type, m_ev_range_type, m_ev_cause, m_ev_flags, m_reserved_1,
        m_reserved_2, m_reserved_3, m_ev_scope_count, m_ev_attachment_count,
        m_ev_creator_index, m_ev_sync_base_value, m_ev_sync_factor);
  }

  data_section_t(const data_section_t &) = default;

  data_section_t(data_section_t &&) = default;

  ~data_section_t() = default;

  data_section_t &operator=(const data_section_t &) = default;

  data_section_t &operator=(data_section_t &&) = default;

  ev_type get_ev_type() const { return m_ev_type; }

  ev_sync_type get_ev_sync_type() const { return m_ev_sync_type; }

  ev_range_type get_ev_range_type() const { return m_ev_range_type; }

  ev_cause get_ev_cause() const { return m_ev_cause; }

  ev_flags get_ev_flags() const { return m_ev_flags; }

  ev_scope_count get_ev_scope_count() const { return m_ev_scope_count; }

  ev_attachment_count get_ev_attachment_count() const {
    return m_ev_attachment_count;
  }

  ev_creator_index get_ev_creator_index() const { return m_ev_creator_index; }

  ev_sync_base_value get_ev_sync_base_value() const {
    return m_ev_sync_base_value;
  }

  ev_sync_factor get_ev_sync_factor() const { return m_ev_sync_factor; }

private:
  using reserved = std::uint8_t;

private:
  ev_type m_ev_type;
  ev_sync_type m_ev_sync_type;
  ev_range_type m_ev_range_type;
  ev_cause m_ev_cause;
  ev_flags m_ev_flags;
  reserved m_reserved_1;
  reserved m_reserved_2;
  reserved m_reserved_3;
  ev_scope_count m_ev_scope_count;
  ev_attachment_count m_ev_attachment_count;
  ev_creator_index m_ev_creator_index;
  ev_sync_base_value m_ev_sync_base_value;
  ev_sync_factor m_ev_sync_factor;
};

std::ostream &operator<<(std::ostream &p_out,
                         const data_section_t<block_id::EV> &p_obj) {
  p_out << "{"
        << "ev_type = " << static_cast<std::uint16_t>(p_obj.get_ev_type())
        << ", ev_sync_type = "
        << static_cast<std::uint16_t>(p_obj.get_ev_sync_type())
        << ", ev_range_type = "
        << static_cast<std::uint16_t>(p_obj.get_ev_range_type())
        << ", ev_cause = " << static_cast<std::uint16_t>(p_obj.get_ev_cause())
        << ", ev_flags = " << static_cast<std::uint16_t>(p_obj.get_ev_flags())
        << ", ev_scope_count  = " << p_obj.get_ev_scope_count()
        << ", ev_attachment_count = " << p_obj.get_ev_attachment_count()
        << ", ev_creator_index = " << p_obj.get_ev_creator_index()
        << ", ev_sync_base_value = " << p_obj.get_ev_sync_base_value()
        << ", ev_sync_factor = " << p_obj.get_ev_sync_factor()

        << "}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
