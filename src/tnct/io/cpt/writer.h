/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_IO_CPT_WRITER_H
#define TNCT_IO_CPT_WRITER_H

#include <concepts>
#include <cstdint>
#include <optional>

#include "tnct/numerate/cpt/is_enum.h"

namespace tnct::io::cpt
{

template <typename t, typename t_error>
concept writer = numerate::cpt::is_enum<t_error> && requires {
  {
    &t::write
  } -> std::same_as<std::optional<t_error> (t::*)(const std::uint8_t *,
                                                  std::size_t)>;
};

}

#endif
