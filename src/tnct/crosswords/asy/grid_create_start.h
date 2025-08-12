#ifndef TENACITAS_SRC_CROSSWORDS_ASY_GRID_CREATE_START_H
#define TENACITAS_SRC_CROSSWORDS_ASY_GRID_CREATE_START_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>

#include "tenacitas/src/crosswords/dat/entries.h"
#include "tenacitas/src/crosswords/dat/index.h"
#include "tenacitas/src/time/cpt/chrono_convertible.h"

namespace tenacitas::src::crosswords::asy {

struct grid_create_start {
  grid_create_start() = default;
  grid_create_start(const grid_create_start &) = default;

  template <tenacitas::src::time::cpt::convertible_to_sec t_duration>
  grid_create_start(const dat::entries &p_entries, dat::index p_num_rows,
                    dat::index p_num_cols, t_duration p_wait_for,
                    dat::index p_max_num_rows)
      : entries(p_entries), num_rows(p_num_rows), num_cols(p_num_cols),
        wait_for(std::chrono::duration_cast<std::chrono::seconds>(p_wait_for)),
        max_num_rows(p_max_num_rows) {}

  dat::entries entries;
  dat::index num_rows;
  dat::index num_cols;
  std::chrono::seconds wait_for;
  dat::index max_num_rows;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_start &) {
    return p_out;
  }
};

} // namespace tenacitas::src::crosswords::asy

#endif
