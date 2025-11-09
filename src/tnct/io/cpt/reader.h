/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_IO_CPT_READER_H
#define TNCT_IO_CPT_READER_H

#include <concepts>
#include <cstdint>
#include <expected>

#include "tnct/numerate/cpt/is_enum.h"

namespace tnct::io::cpt
{

template <typename t, typename t_error>
concept reader = numerate::cpt::is_enum<t_error> && requires {
  {
    &t::read
  } -> std::same_as<std::expected<std::size_t, t_error> (t::*)(std::uint8_t *,
                                                               std::size_t)>;
};

}

#endif
