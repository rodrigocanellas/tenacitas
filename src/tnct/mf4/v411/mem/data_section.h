/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_DATA_SECTION_H
#define TNCT_MF4_V411_MEM_DATA_SECTION_H

#include <iostream>

#include "tnct/mf4/v411/mem/block_id.h"

namespace tnct::mf4::v411::mem {

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

template <> struct data_section_t<block_id::CH> final {
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
  p_out << "{}";
  return p_out;
}

} // namespace tnct::mf4::v411::mem

#endif
