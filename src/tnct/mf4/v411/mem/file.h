/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MF4_V411_MEM_FILE_H
#define TNCT_MF4_V411_MEM_FILE_H

#include <optional>
#include <tuple>
#include <vector>

#include "tnct/format/fmt.h"
#include "tnct/generic/log_and_throw.h"
#include "tnct/mf4/v411/mem/block.h"
#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/block_ref.h"
#include "tnct/traits/log/logger.h"
#include "tnct/traits/tuple/find.h"

namespace tnct::mf4::v411::mem {

struct file {
  template <block_id t_block_id, tnct::traits::log::logger t_logger>
  block_index add(std::optional<block_ref> p_parent, t_logger &p_logger) {
    constexpr std::optional<size_t> _optional_index =
        traits::tuple::find<blocks_catalog, blocks_t<t_block_id>>();

    if (!_optional_index.has_value()) {
      generic::log_and_throw(
          p_logger,
          format::fmt("block_id ", block_id_converter::to_str(t_block_id),
                      " was not found in the std::tuple of block ids"));
    }

    constexpr std::size_t _blocks_index{_optional_index.value()};
    using blocks = std::tuple_element_t<_blocks_index, blocks_catalog>;

    blocks &_blocks = std::get<_blocks_index>(m_blocks_catalog);

    std::size_t _block_index = _blocks.size();
    _blocks.push_back({_block_index, p_parent});
    return _block_index;
  }

  template <block_id t_block_id, tnct::traits::log::logger t_logger>
  block_t<t_block_id> &get(block_index p_block_index, t_logger &p_logger) {
    constexpr std::optional<size_t> _optional_index =
        traits::tuple::find<blocks_catalog, blocks_t<t_block_id>>();

    if (!_optional_index.has_value()) {
      generic::log_and_throw(
          p_logger,
          format::fmt("block_id ", block_id_converter::to_str(t_block_id),
                      " was not found in the std::tuple of block ids"));
    }

    constexpr std::size_t _blocks_index{_optional_index.value()};
    using blocks = std::tuple_element_t<_blocks_index, blocks_catalog>;

    blocks &_blocks = std::get<_blocks_index>(m_blocks_catalog);

    if (p_block_index >= _blocks.size()) {
      generic::log_and_throw(
          p_logger, format::fmt("block index ", p_block_index,
                                " is out of range for block_id ",
                                block_id_converter::to_str(t_block_id),
                                " which has ", _blocks.size(), " blocks "));
    }

    return _blocks[p_block_index];
  }

  template <tnct::traits::log::logger t_logger>
  void report(std::ostream &p_out, t_logger &p_logger) {
    if (std::tuple_size_v<blocks_catalog> == 0) {
      generic::log_and_throw(p_logger, "no block id defined in the tuple");
    }
    blocks_t<block_id::HD> &_blocks{std::get<0>(m_blocks_catalog)};

    block_t<block_id::HD> &_block(_blocks[0]);

    block_ref _block_ref{_block.get_id(), _block.get_index()};

    print(p_out, _block_ref, p_logger);
  }

private:
  template <tnct::traits::log::logger t_logger>
  void print(std::ostream &p_out, const block_ref &p_block_ref,
             t_logger &p_logger, std::size_t p_level = 0) {
    auto _print = [&]<block_id t_block_id>() {
      using block = block_t<t_block_id>;
      block &_block{get<t_block_id>(p_block_ref.get_index(), p_logger)};

      p_out << std::string(p_level, '\t') << "{"
            << block_id_converter::to_str(_block.get_id()) << ','
            << _block.get_index() << "}\n";
      using const_links_iterator = typename block::const_links_iterator;
      for (const_links_iterator _ite = _block.begin(); _ite != _block.end();
           ++_ite) {
        print(p_out, {_ite->get_id(), _ite->get_index()}, p_logger,
              p_level + 1);
      }
    };

    visit_blocks_ids(p_block_ref.get_id(), _print);
  }

private:
  template <block_id t_block_id>
  using blocks_t = std::vector<block_t<t_block_id>>;

  using blocks_catalog = std::tuple<
      blocks_t<block_id::HD>, blocks_t<block_id::MD>, blocks_t<block_id::TX>,
      blocks_t<block_id::FH>, blocks_t<block_id::CH>, blocks_t<block_id::AT>,
      blocks_t<block_id::EV>, blocks_t<block_id::DG>, blocks_t<block_id::CG>,
      blocks_t<block_id::SI>, blocks_t<block_id::CN>, blocks_t<block_id::CC>,
      blocks_t<block_id::CA>, blocks_t<block_id::DT>, blocks_t<block_id::SR>,
      blocks_t<block_id::RD>, blocks_t<block_id::SD>, blocks_t<block_id::DL>,
      blocks_t<block_id::DZ>, blocks_t<block_id::HL>>;

private:
  blocks_catalog m_blocks_catalog;
};

} // namespace tnct::mf4::v411::mem

#endif
