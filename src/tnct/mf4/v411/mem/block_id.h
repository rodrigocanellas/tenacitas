/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_BLOCK_ID_H
#define TNCT_MF4_V411_MEM_BLOCK_ID_H

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <optional>
#include <type_traits>

namespace tnct::mf4::v411::mem {

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
                 static_cast<std::underlying_type_t<block_id>>(block_id::HL) +
                     1>;

  using block_id_type = std::underlying_type_t<block_id>;

  static constexpr array m_array{"##ID", "##HD", "##MD", "##TX", "##FH", "##CH",
                                 "##AT", "##EV", "##DG", "##CG", "##SI", "##CN",
                                 "##CC", "##CA", "##DT", "##SR", "##RD", "##SD",
                                 "##DL", "##DZ", "##HL"};
};

template <typename t_func>
void visit_blocks_ids(block_id p_block_id, t_func p_func)

{
  switch (p_block_id) {
  case mem::block_id::HD:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::HD>();
                  }) {
      p_func.template operator()<mem::block_id::HD>();
    }
    break;

  case mem::block_id::MD:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::MD>();
                  }) {
      p_func.template operator()<mem::block_id::MD>();
    }
    break;

  case mem::block_id::TX:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::TX>();
                  }) {
      p_func.template operator()<mem::block_id::TX>();
    }
    break;

  case mem::block_id::FH:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::FH>();
                  }) {
      p_func.template operator()<mem::block_id::FH>();
    }
    break;

  case mem::block_id::CH:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::CH>();
                  }) {
      p_func.template operator()<mem::block_id::CH>();
    }
    break;

  case mem::block_id::AT:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::AT>();
                  }) {
      p_func.template operator()<mem::block_id::AT>();
    }
    break;

  case mem::block_id::EV:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::EV>();
                  }) {
      p_func.template operator()<mem::block_id::EV>();
    }
    break;

  case mem::block_id::DG:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::DG>();
                  }) {
      p_func.template operator()<mem::block_id::DG>();
    }
    break;

  case mem::block_id::CG:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::CG>();
                  }) {
      p_func.template operator()<mem::block_id::CG>();
    }
    break;

  case mem::block_id::SI:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::SI>();
                  }) {
      p_func.template operator()<mem::block_id::SI>();
    }
    break;

  case mem::block_id::CN:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::CN>();
                  }) {
      p_func.template operator()<mem::block_id::CN>();
    }
    break;

  case mem::block_id::CC:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::CC>();
                  }) {
      p_func.template operator()<mem::block_id::CC>();
    }
    break;

  case mem::block_id::CA:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::CA>();
                  }) {
      p_func.template operator()<mem::block_id::CA>();
    }
    break;

  case mem::block_id::DT:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::DT>();
                  }) {
      p_func.template operator()<mem::block_id::DT>();
    }
    break;

  case mem::block_id::SR:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::SR>();
                  }) {
      p_func.template operator()<mem::block_id::SR>();
    }
    break;

  case mem::block_id::RD:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::RD>();
                  }) {
      p_func.template operator()<mem::block_id::RD>();
    }
    break;

  case mem::block_id::SD:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::SD>();
                  }) {
      p_func.template operator()<mem::block_id::SD>();
    }
    break;

  case mem::block_id::DL:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::DL>();
                  }) {
      p_func.template operator()<mem::block_id::DL>();
    }
    break;

  case mem::block_id::DZ:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::DZ>();
                  }) {
      p_func.template operator()<mem::block_id::DZ>();
    }
    break;

  case mem::block_id::HL:
    if constexpr (requires {
                    p_func.template operator()<mem::block_id::HL>();
                  }) {
      p_func.template operator()<mem::block_id::HL>();
    }
    break;

  default:
    break;
  }
}

} // namespace tnct::mf4::v411::mem

#endif
