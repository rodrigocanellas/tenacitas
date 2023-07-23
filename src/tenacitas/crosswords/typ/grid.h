#ifndef TENACITAS_LIB_CROSSWORDS_TYP_CROSSWORDS_H
#define TENACITAS_LIB_CROSSWORDS_TYP_CROSSWORDS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <iterator>
#include <limits>
#include <optional>

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <tenacitas/container/typ/matrix.h>
#include <tenacitas/log/alg/logger.h>

namespace tenacitas::lib::crosswords::typ {

/// \brief Index in a grid
using index = int16_t;

/// \brief Word to be positioned in a grid
using word = std::string;

/// \brief Helper function to get the size of a word
index get_size(const word &p_word) { return static_cast<index>(p_word.size()); }

/// \brief Maximum length of a \p word
static constexpr word::value_type max_char{
    std::numeric_limits<word::value_type>::max()};

/// \brief Explanation of a word
using explanation = std::string;

/// \brief A \p word and its \p explanation
struct entry {
  entry() = delete;

  entry(const entry &) = default;

  entry(entry &&) = default;

  entry(word &&p_word, explanation &&p_explanation)
      : m_word(std::move(p_word)), m_explanation(std::move(p_explanation)) {}

  ~entry() = default;

  entry &operator=(const entry &) = default;
  entry &operator=(entry &&) = default;

  inline const word &get_word() const { return m_word; }

  inline const explanation &get_explanation() const { return m_explanation; }

private:
  word m_word;
  explanation m_explanation;
};

static const entry empty_entry{"", ""};

/// \brief A set of \p entry
struct entries {

  using size = uint8_t;
  using collection = std::vector<entry>;
  using const_entry_ite = collection::const_iterator;
  using entry_ite = collection::iterator;

  entries() = default;
  entries(const entries &) = default;
  entries(entries &) = default;
  inline entries(std::initializer_list<entry> &&p_enties) {
    m_collection = std::move(p_enties);
  }

  ~entries() = default;

  entries &operator=(const entries &) = default;
  entries &operator=(entries &) = default;

  inline void add_entry(word &&p_word, explanation &&p_explanation) {
    m_collection.push_back({std::move(p_word), std::move(p_explanation)});
  }

  inline void add_entry(entry &&p_entry) {
    m_collection.push_back(std::move(p_entry));
  }

  inline const_entry_ite begin() const { return m_collection.begin(); }
  inline const_entry_ite end() const { return m_collection.end(); }

  inline entry_ite begin() { return m_collection.begin(); }
  inline entry_ite end() { return m_collection.end(); }

  inline size get_num_entries() const {
    return static_cast<size>(m_collection.size());
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const entries &p_entries) {
    p_out << "\nENTRIES\n";
    for (const entry &_entry : p_entries) {
      p_out << _entry.get_word() << '\t' << _entry.get_explanation() << '\n';
    }
    return p_out;
  }

private:
  collection m_collection;
};

/// \brief A combination of entries
using permutation = std::vector<entries::const_entry_ite>;

std::ostream &operator<<(std::ostream &p_out,
                         const permutation &p_permutation) {
  p_out << "{ ";
  for (entries::const_entry_ite _entry : p_permutation) {
    p_out << _entry->get_word() << ' ';
  }
  p_out << '}';
  return p_out;
}

static constexpr index max_row{std::numeric_limits<index>::max()};
static constexpr index max_col{std::numeric_limits<index>::max()};

/// \brief Possible orientations of a \p word in a grid
enum class orientation : char { vert = 'V', hori = 'H', undef = 'U' };

std::ostream &operator<<(std::ostream &p_out, orientation p_orientation) {
  p_out << static_cast<char>(p_orientation);
  return p_out;
}

/// \brief Row and Column
using coordinate = std::pair<index, index>;

/// \brief Set of rows and columns
using coordinates = std::vector<coordinate>;

/// \brief An \p entry with a \p orientation and \p coordinate defined
struct layout {
  layout() = default;

  layout(entries::const_entry_ite p_entry) : m_entry(p_entry) {}

  layout(const layout &) = default;
  layout(layout &&) = default;

  ~layout() = default;

  layout &operator=(const layout &) = default;
  layout &operator=(layout &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const layout &p_layout) {
    p_out << "('" << p_layout.m_entry->get_word() << "',"
          << static_cast<uint16_t>(p_layout.m_row) << ','
          << static_cast<uint16_t>(p_layout.m_col) << ','
          << p_layout.m_orientation << ')';

    return p_out;
  }

  inline const word &get_word() const { return m_entry->get_word(); }
  inline index get_row() const { return m_row; }
  inline index get_col() const { return m_col; }
  inline orientation get_orientation() const { return m_orientation; }
  inline void set_entry(entries::const_entry_ite p_entry) { m_entry = p_entry; }
  inline void set_row(index p_x) { m_row = p_x; }
  inline void set_col(index p_y) { m_col = p_y; }
  inline void set_orientation(orientation p_orientation) {
    m_orientation = p_orientation;
  }
  inline bool is_positioned() const {
    return m_orientation != orientation::undef;
  }

  void reset() {
    m_row = max_row;
    m_col = max_col;
    m_orientation = orientation::undef;
  }

private:
  entries::const_entry_ite m_entry;
  index m_row{max_row};
  index m_col{max_col};
  orientation m_orientation{orientation::undef};
};

/// \brief Defines which coordinates are occupied
using occupied = lib::container::typ::matrix<index, word::value_type>;

/// \brief Contains all the \p layout
struct grid {
  using layouts = std::vector<layout>;
  using const_layout_ite = layouts::const_iterator;
  using layout_ite = layouts::iterator;

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
  grid(const permutation &p_permutation, index p_num_rows, index p_num_cols,
       uint64_t p_permutation_number = 0)
      : m_longest(longest_word(p_permutation)), m_num_rows(p_num_rows),
        m_num_cols(p_num_cols), m_permutation_number(p_permutation_number),
        m_occupied(p_num_rows, p_num_cols, max_char) {

    // checks if all the words fit in the grid
    if ((m_longest > p_num_rows) && (m_longest > p_num_cols)) {
      std::string _err("Longest word has " + std::to_string(m_longest) +
                       " chars, and is longer than " +
                       std::to_string(p_num_rows) + " rows and " +
                       std::to_string(p_num_cols) + " columns");
      TNCT_LOG_ERR(_err);
      throw std::runtime_error(_err);
    }

    // fills the collection of \p layout objects
    for (entries::const_entry_ite _entry : p_permutation) {
      m_layouts.push_back(_entry);
    }

    // row header format used when printing the grid to console
    {
      std::stringstream _stream;
      _stream << ' ';
      for (index _col = 0; _col < m_occupied.get_num_cols(); ++_col) {
        _stream << "+-";
      }
      _stream << "+\n";
      m_horizontal_line = _stream.str();
    }

    // column header format used when printing the grid to console
    {
      std::stringstream _stream;
      _stream << ' ';
      for (index _col = 0; _col < m_occupied.get_num_cols(); ++_col) {
        _stream << ' ' << std::hex << std::uppercase << _col;
      }
      _stream << '\n';

      m_header = _stream.str();
    }
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
  friend std::ostream &operator<<(std::ostream &p_out, const grid &p_grid) {
    p_out << '\n';

    const occupied &_occupied = p_grid.m_occupied;

    index _row_size = _occupied.get_num_rows();
    index _col_size = _occupied.get_num_cols();

    p_out << p_grid.m_header << p_grid.m_horizontal_line;

    for (index _row = 0; _row < _row_size; ++_row) {
      p_out << std::hex << std::uppercase << _row << "|";
      for (index _col = 0; _col < _col_size; ++_col) {
        auto _c{_occupied(_row, _col)};
        p_out << (_c == std::numeric_limits<word::value_type>::max() ? ' ' : _c)
              << '|';
      }
      p_out << '\n' << p_grid.m_horizontal_line;
    }
    return p_out;
  }

  inline uint64_t get_permutation_number() const {
    return m_permutation_number;
  }

  inline index get_num_rows() const { return m_num_rows; }
  inline index get_num_cols() const { return m_num_cols; }

  inline layout_ite begin() { return m_layouts.begin(); }
  inline layout_ite end() { return m_layouts.end(); }
  inline bool empty() const { return m_layouts.empty(); }

  inline const_layout_ite begin() const { return m_layouts.begin(); }
  inline const_layout_ite end() const { return m_layouts.end(); }

  void set(layouts::iterator p_ite, index p_row, index p_col,
           orientation p_orientation) {
    p_ite->set_row(p_row);
    p_ite->set_col(p_col);
    p_ite->set_orientation(p_orientation);
    occupy(p_ite);
  }

  bool organized() const {
    for (const layout &_layout : m_layouts) {
      if (_layout.get_orientation() == orientation::undef) {
        return false;
      }
    }
    return true;
  }

  void reset_positions() {
    for (layout &_layout : m_layouts) {
      _layout.reset();
    }
    m_occupied.reset();
  }

  inline std::optional<word::value_type> is_occupied(index p_row, index p_col) {
    word::value_type _c = m_occupied(p_row, p_col);
    if (_c == max_char) {
      return {};
    }
    return {_c};
  }

  inline index longest_word() const { return m_longest; }

private:
  void occupy(const_layout_ite p_layout) {
    index _count = 0;
    if (p_layout->get_orientation() == orientation::vert) {
      for (word::value_type _c : p_layout->get_word()) {
        m_occupied(p_layout->get_row() + _count++, p_layout->get_col()) = _c;
      }
    } else {
      for (word::value_type _c : p_layout->get_word()) {
        m_occupied(p_layout->get_row(), p_layout->get_col() + _count++) = _c;
      }
    }
  }

  index longest_word(const permutation &p_permutation) {
    using namespace typ;
    index _size{0};

    for (entries::const_entry_ite _entry : p_permutation) {
      auto _current{get_size(_entry->get_word())};
      if (_current > _size) {
        _size = _current;
      }
    }

    return _size;
  }

private:
  index m_longest{0};
  index m_num_rows{0};
  index m_num_cols{0};
  uint64_t m_permutation_number;

  occupied m_occupied;
  layouts m_layouts;

  std::string m_header;
  std::string m_horizontal_line;
};

} // namespace tenacitas::lib::crosswords::typ

#endif // CROSSWORDS_H
