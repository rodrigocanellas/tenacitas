#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_TIMEOUT_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_TIMEOUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

// #include <iostream>
// #include <optional>

// #include "tnct/crosswords/dat/index.h"

namespace tnct::crosswords::evt
{

// struct grid_create_timeout
// {
//   grid_create_timeout()                            = default;
//   grid_create_timeout(const grid_create_timeout &) = default;
//   grid_create_timeout(grid_create_timeout &&)      = default;

//   grid_create_timeout &operator=(const grid_create_timeout &) = default;
//   grid_create_timeout &operator=(grid_create_timeout &&)      = default;

//   ~grid_create_timeout() = default;

//   grid_create_timeout(dat::index p_num_rows, dat::index p_num_cols,
//                       std::optional<std::size_t> p_client = std::nullopt)
//       : num_rows(p_num_rows), num_cols(p_num_cols), client(p_client)
//   {
//   }

//   friend std::ostream &
//   operator<<(std::ostream              &p_out,
//              const grid_create_timeout &p_grid_create_timeout)
//   {
//     p_out << "grid_create_timeout for grid " <<
//     p_grid_create_timeout.num_rows
//           << 'x' << p_grid_create_timeout.num_cols;
//     return p_out;
//   }

//   dat::index                 num_rows{dat::invalid_index};
//   dat::index                 num_cols{dat::invalid_index};
//   std::optional<std::size_t> client = std::nullopt;
// };

} // namespace tnct::crosswords::evt

#endif
