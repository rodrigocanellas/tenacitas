/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_PER_BLOCK_H
#define TNCT_MF4_PER_BLOCK_H

#include <fstream>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/file.h"
#include "tnct/mf4/v411/per/basic_types.h"
#include "tnct/mf4/v411/per/data_section.h"
#include "tnct/mf4/v411/per/header_section.h"
#include "tnct/traits/log/logger.h"

namespace tnct::mf4::v411::per {

template <mem::block_id t_block_id> struct block_reader_t {
  template <tnct::traits::log::logger t_logger>
  mem::block_ref operator()(std::ifstream &p_file, t_logger &p_logger,
                            int64_t p_offset_start, amount_to_read p_block_size,
                            std::size_t p_num_links, mem::file &p_mf4_file,
                            std::optional<mem::block_ref> p_parent,
                            std::size_t p_level = 0) {

    const amount_to_read size_to_read{p_block_size - header_section_size};

    const offset _start{p_offset_start +
                        static_cast<offset>(header_section_size)};

    if (!p_file.seekg(_start).good()) {
      log_and_throw(p_logger,
                    format::fmt("error seekg for ", _start, " bytes"));
    }

    std::vector<char> _buf(size_to_read, 0);
    if (!p_file.read(_buf.data(), size_to_read).good()) {
      log_and_throw(p_logger, format::fmt("error reading ", size_to_read,
                                          " bytes for header block "));
    }

    mem::block_index _block_index{
        p_mf4_file.add<t_block_id>(p_parent, p_logger)};

    mem::block_ref _block_ref{t_block_id, _block_index};

    auto _ptr = reinterpret_cast<const std::uint8_t *>(&_buf[0]);
    {
      for (decltype(p_num_links) _i = 0; _i < p_num_links; ++_i) {

        offset _offset{
            byte_array::from_little<offset>(_ptr + _i * sizeof(offset))};

        if (_offset == 0) {
          continue;
        }

        mem::block_ref _inner_block_ref{read_block<t_logger>(
            p_file, p_logger, _offset, p_mf4_file, {_block_ref}, p_level + 1)};

        p_mf4_file.get<t_block_id>(_block_index, p_logger)
            .add_link(std::move(_inner_block_ref));
      }
    }

    _ptr += (p_num_links * sizeof(mem::block_index));

    data_section_t<t_block_id> _data_section_reader;

    auto &_block{p_mf4_file.get<t_block_id>(_block_index, p_logger)};
    _block.set_data_section(std::move(_data_section_reader(_ptr)));

    // TNCT_LOG_INF(p_logger, format::fmt(std::string(p_level, '\t'), _block));

    return _block_ref;
  }
};

template <tnct::traits::log::logger t_logger>
mem::block_ref read_block(std::ifstream &p_file, t_logger &p_logger,
                          offset p_offset_start, mem::file &p_mf4_file,
                          std::optional<mem::block_ref> p_parent = std::nullopt,
                          std::size_t p_level = 0) {
  if (!p_file.seekg(p_offset_start).good()) {
    log_and_throw(p_logger,
                  format::fmt("error seekg for ", p_offset_start, " bytes"));
  }

  auto [_block_id, _block_size, _block_num_links] =
      read_header_section<t_logger>(p_file, p_logger, p_offset_start);

  mem::block_ref _block_ref;

  auto _block_reader = [&]<mem::block_id t_block_id>() {
    _block_ref = block_reader_t<t_block_id>()(p_file, p_logger, p_offset_start,
                                              _block_size, _block_num_links,
                                              p_mf4_file, p_parent, p_level);
  };

  mem::visit_blocks_ids(_block_id, _block_reader);

  return _block_ref;
}

} // namespace tnct::mf4::v411::per

#endif
