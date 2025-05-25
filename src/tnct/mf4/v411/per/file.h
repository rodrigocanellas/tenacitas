/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_FILE_H
#define TNCT_MF4_PER_FILE_H

#include <cstring>
#include <fstream>

#include "tnct/format/fmt.h"
#include "tnct/mf4/v411/log_and_throw.h"
#include "tnct/mf4/v411/mem/file.h"
#include "tnct/mf4/v411/per/block.h"
#include "tnct/mf4/v411/per/id_block.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::per {

template <tnct::traits::log::logger t_logger>
void read(std::string_view file_name, t_logger &p_logger,
                mem::file &p_mf4_file) {

  std::ifstream _file(file_name.data(), std::ios_base::binary);
  if (!_file.good()) {
    log_and_throw(p_logger, format::fmt("file ", file_name.data(),
                                        " could not be opened"));
  }

  try {
    auto [_id_block, _id_block_size] = read_id_block(_file, p_logger);
    TNCT_LOG_INF(p_logger,
                 format::fmt(_id_block, " - size = ", _id_block_size));

    read_block(_file, p_logger, _id_block_size, p_mf4_file);

  } catch (std::exception &_ex) {
    log_and_throw(p_logger, _ex.what());
  } catch (...) {
    log_and_throw(p_logger, "ERROR unknow");
  }
}

} // namespace tnct::mf4::v411::per

#endif
