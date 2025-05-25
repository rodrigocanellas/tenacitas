#ifndef TNCT_MF4_V411_MEM_CLASSES_BLOCK_H
#define TNCT_MF4_V411_MEM_CLASSES_BLOCK_H

#include <iostream>

#include "tnct/mf4/v411/mem/block_id.h"
#include "tnct/mf4/v411/mem/block_index.h"
#include "tnct/mf4/v411/mem/block_ref.h"
#include "tnct/mf4/v411/mem/data_section.h"
#include "tnct/mf4/v411/mem/data_section_hd.h"

namespace tnct::mf4::v411::mem {

template <block_id t_block_id> struct block_t final {

  using id = block_id;

  using block_ids_links = std::vector<block_ref>;

  using const_links_iterator = block_ids_links::const_iterator;

  using data_section = data_section_t<t_block_id>;

  block_t() = delete;

  block_t(block_index p_position,
          const std::optional<block_ref> &p_parent = std::nullopt)
      : m_index(p_position), m_parent(p_parent) {}

  block_t(const block_t &) = delete;

  block_t(block_t &&) noexcept = default;

  ~block_t() = default;

  block_t &operator=(const block_t &) = delete;
  block_t &operator=(block_t &&) noexcept = default;

  constexpr bool operator==(const block_t &p_block) const {
    return (m_id == p_block.m_id) && (m_index == p_block.m_index);
  }

  constexpr bool operator!=(const block_t &p_block) const {
    return !(*this == p_block);
  }

  constexpr bool operator<(const block_t &p_block) const {
    if (m_id < p_block.m_id) {
      return true;
    }
    if (m_id > p_block.m_id) {
      return false;
    }
    if (m_index < p_block.m_index) {
      return true;
    }
    return false;
  }

  const block_id &get_id() const { return m_id; }

  const block_index &get_index() const { return m_index; }

  std::optional<block_ref> get_parent() const { return m_parent; }

  const_links_iterator begin() const { return m_block_ids_links.begin(); }

  const_links_iterator end() const { return m_block_ids_links.end(); }

  std::size_t get_num_links() const { return m_block_ids_links.size(); }

  void add_link(block_ref &&p_link) {
    m_block_ids_links.push_back(std::move(p_link));
  }

  void set_data_section(data_section &&p_data_section) {
    m_data_section = std::move(p_data_section);
  }

  const data_section &get_data_section() const { return m_data_section; }

private:
  block_id m_id{t_block_id};

  block_index m_index;

  std::optional<block_ref> m_parent;

  block_ids_links m_block_ids_links;

  data_section m_data_section{};
};

// referencing and unique link?

template <block_id t_block_id>
std::ostream &operator<<(std::ostream &p_out,
                         const block_t<t_block_id> &p_block) {
  p_out << "{ {id " << block_id_converter::to_str(p_block.get_id())
        << "} {position " << p_block.get_index() << "} {parent ";
  if (p_block.get_parent().has_value()) {
    p_out << p_block.get_parent().value();
  } else {
    p_out << "{NULL}";
  }
  p_out << "} {links ";
  for (typename block_t<t_block_id>::const_links_iterator _ite =
           p_block.begin();
       _ite != p_block.end(); ++_ite) {
    p_out << *_ite;
  }
  p_out << "}";
  p_out << " {data " << p_block.get_data_section() << "} }";
  return p_out;
}

} // namespace tnct::mf4::v411::mem
#endif
