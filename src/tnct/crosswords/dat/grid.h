#ifndef TNCT_CROSSWORDS_DAT_CROSSWORDS_H
#define TNCT_CROSSWORDS_DAT_CROSSWORDS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "tnct/crosswords/dat/entries.h"
#include "tnct/crosswords/dat/entry.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/crosswords/dat/layout.h"
#include "tnct/crosswords/dat/occupied.h"
#include "tnct/crosswords/dat/orientation.h"
#include "tnct/crosswords/dat/permutation.h"
#include "tnct/crosswords/dat/word.h"

namespace tnct::crosswords::dat
{

/// \brief Contains all the \p layout
struct grid
{
  using layouts          = std::vector<layout>;
  using const_layout_ite = layouts::const_iterator;
  using layout_ite       = layouts::iterator;

  grid() = default;

  /// \brief Constructor
  ///
  /// \param p_permutation is a permutation of the \p entries to be used when
  /// trying to assemble the grid
  ///
  /// \param p_num_rows number of rows in the grid
  ///
  /// \param p_num_cols number of columns in the grid
  ///
  /// \param p_permutation_number number of permutation of a \p entries used
  grid(std::shared_ptr<const entries> p_entries,
       const permutation &p_permutation, index p_num_rows, index p_num_cols,
       uint64_t p_permutation_number = 0)
      : m_longest(longest_word(p_permutation)), m_num_rows(p_num_rows),
        m_num_cols(p_num_cols), m_permutation_number(p_permutation_number),
        m_occupied(p_num_rows, p_num_cols, dat::max_char), m_entries(p_entries)
  {

    // checks if all the words fit in the grid
    if ((m_longest > p_num_rows) && (m_longest > p_num_cols))
    {
      std::string _err("Longest word has " + std::to_string(m_longest)
                       + " chars and is longer than "
                       + std::to_string(p_num_rows) + " rows and "
                       + std::to_string(p_num_cols) + " columns");
      throw std::runtime_error(_err);
    }

    // fills the collection of \p layout objects
    uint16_t _id{1};
    for (entries::const_entry_ite _entry : p_permutation)
    {
      m_layouts.emplace_back(_entry, _id++);
    }

    // row header format used when printing the grid to console
    {
      std::stringstream _stream;
      _stream << "  ";
      for (index _col = 0; _col < m_occupied.get_num_cols(); ++_col)
      {
        _stream << "+--";
      }
      _stream << "+\n";
      m_horizontal_line = _stream.str();
    }

    // column header format used when printing the grid to console
    {
      std::stringstream _stream;
      _stream << "  ";
      for (index _col = 0; _col < m_occupied.get_num_cols(); ++_col)
      {
        _stream << ' ' << std::setw(2) << _col;
      }
      _stream << '\n';

      m_header = _stream.str();
    }
  }

  grid(const grid &p_grid)
      : m_longest(p_grid.m_longest), m_num_rows(p_grid.m_num_rows),
        m_num_cols(p_grid.m_num_cols),
        m_permutation_number(p_grid.m_permutation_number),
        m_occupied(p_grid.m_occupied), m_entries(p_grid.m_entries),
        m_layouts(p_grid.m_layouts), m_header(p_grid.m_header),
        m_horizontal_line(p_grid.m_horizontal_line)
  {
  }

  grid &operator=(const grid &p_grid)
  {
    if (this != &p_grid)
    {
      m_longest            = p_grid.m_longest;
      m_num_rows           = p_grid.m_num_rows;
      m_num_cols           = p_grid.m_num_cols;
      m_permutation_number = p_grid.m_permutation_number;
      m_occupied           = p_grid.m_occupied;
      m_layouts            = p_grid.m_layouts;
      m_header             = p_grid.m_header;
      m_horizontal_line    = p_grid.m_horizontal_line;
    }
    return *this;
  }

  std::optional<uint16_t> is_first_letter(dat::index p_row, dat::index p_col)
  {
    for (const_layout_ite _ite = m_layouts.begin(); _ite != m_layouts.end();
         ++_ite)
    {
      if ((_ite->get_row() == p_row) && (_ite->get_col() == p_col))
      {
        return {_ite->get_id()};
      }
    }
    return {};
  }

  /// \brief Prints the grid to the console, like
  ///
  ///  0 1 2 3 4 5 6 7 8 9 A
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 0|d|e|b|u|t|e| | |r| |a|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 1| |v|a|r|e|s|t|a|a| |g|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 2|s|i|d|e|r|a|l|e|p| |u|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 3|i|r|a|n| |l| |x|i| |i|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 4|b|a|l|o|l|u|f|u|n|c|p|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 5|l|v|a|v|e|t|a|m|a|r|a|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 6|i|i|r|a|s|a|r|a| |e|v|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 7|a|r| |r|a|r|e|r| |p|i|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 8|r|a|f|u|n|i|l|a|r|o|v|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// 9| |t|e|a|t|r|o| | |m|a|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  /// A| | | | |e| |s| | | |r|
  /// +-+-+-+-+-+-+-+-+-+-+-+
  ///
  friend std::ostream &operator<<(std::ostream &p_out, const grid &p_grid)
  {
    p_out << '\n';

    const occupied &_occupied = p_grid.m_occupied;

    index _row_size = _occupied.get_num_rows();
    index _col_size = _occupied.get_num_cols();

    p_out << p_grid.m_header << p_grid.m_horizontal_line;

    for (index _row = 0; _row < _row_size; ++_row)
    {
      p_out << std::setw(2) << _row << "|";
      for (index _col = 0; _col < _col_size; ++_col)
      {
        auto _c{_occupied(_row, _col)};
        if (_c == dat::max_char)
        {
          p_out << "  ";
        }
        else
        {
          p_out << ' ' << _c;
        }
        p_out << '|';
      }
      p_out << '\n' << p_grid.m_horizontal_line;
    }
    return p_out;
  }

  const occupied &get_occupied() const
  {
    return m_occupied;
  }

  inline uint64_t get_permutation_number() const
  {
    return m_permutation_number;
  }

  inline index get_num_rows() const
  {
    return m_num_rows;
  }
  inline index get_num_cols() const
  {
    return m_num_cols;
  }

  inline layout_ite begin()
  {
    return m_layouts.begin();
  }
  inline layout_ite end()
  {
    return m_layouts.end();
  }
  inline bool empty() const
  {
    return m_layouts.empty();
  }

  inline const_layout_ite begin() const
  {
    return m_layouts.begin();
  }
  inline const_layout_ite end() const
  {
    return m_layouts.end();
  }

  void set(layouts::iterator p_ite, index p_row, index p_col,
           orientation p_orientation)
  {
    p_ite->set_row(p_row);
    p_ite->set_col(p_col);
    p_ite->set_orientation(p_orientation);
    occupy(p_ite);
  }

  bool organized() const
  {
    for (const layout &_layout : m_layouts)
    {
      if (_layout.get_orientation() == orientation::undef)
      {
        return false;
      }
    }
    return true;
  }

  void reset_positions()
  {
    for (layout &_layout : m_layouts)
    {
      _layout.reset();
    }
    m_occupied.reset();
  }

  inline std::optional<word::value_type> is_occupied(index p_row, index p_col)
  {
    word::value_type _c = m_occupied(p_row, p_col);
    if (_c == dat::max_char)
    {
      return {};
    }
    return {_c};
  }

  inline index longest_word() const
  {
    return m_longest;
  }

  std::optional<const_layout_ite> get_id(index p_row, index p_col) const
  {
    for (auto _ite = begin(); _ite != end(); ++_ite)
    {
      if (_ite->get_id(p_row, p_col))
      {
        return {_ite};
      }
    }
    return {};
  }

  orientation get_orientation(uint16_t p_id) const
  {
    orientation _orientation{orientation::undef};
    for (auto _ite = begin(); _ite != end(); ++_ite)
    {
      if (_ite->get_id() == p_id)
      {
        _orientation = _ite->get_orientation();
      }
    }
    return _orientation;
  }

private:
  void occupy(const_layout_ite p_layout)
  {
    index _count = 0;
    if (p_layout->get_orientation() == orientation::vert)
    {
      for (index _i = 0; _i < static_cast<index>(p_layout->get_word().size());
           ++_i)
      {
        m_occupied(p_layout->get_row() + _count++, p_layout->get_col()) =
            p_layout->get_word()[_i];
      }

      //      for (word::value_type _c : p_layout->get_word()) {
      //        m_occupied(p_layout->get_row() + _count++, p_layout->get_col())
      //        = _c;
      //      }
    }
    else
    {
      for (index _i = 0; _i < static_cast<index>(p_layout->get_word().size());
           ++_i)
      {
        m_occupied(p_layout->get_row(), p_layout->get_col() + _count++) =
            p_layout->get_word()[_i];
      }
      //      for (word::value_type _c : p_layout->get_word()) {
      //        m_occupied(p_layout->get_row(), p_layout->get_col() + _count++)
      //        = _c;
      //      }
    }
  }

  index longest_word(const permutation &p_permutation)
  {
    // using namespace dat;
    index _size{0};

    for (entries::const_entry_ite _entry : p_permutation)
    {
      auto _current{_entry->get_word().size()};
      if (static_cast<index>(_current) > _size)
      {
        _size = _current;
      }
    }

    return _size;
  }

private:
  index    m_longest{0};
  index    m_num_rows{0};
  index    m_num_cols{0};
  uint64_t m_permutation_number;

  occupied                       m_occupied;
  std::shared_ptr<const entries> m_entries;
  layouts                        m_layouts;

  std::string m_header;
  std::string m_horizontal_line;
};

} // namespace tnct::crosswords::dat

#endif // CROSSWORDS_H
