/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_MATRIX_H
#define TNCT_CONTAINER_DAT_MATRIX_H

#include <algorithm>
#include <cstring>
#include <memory>
#include <optional>

namespace tnct::container::dat
{

template <std::integral t_index, std::copyable t_data>
struct matrix final
{
  using index = t_index;
  using data  = t_data;

  matrix() = default;

  matrix(const matrix &p_matrix)
      : m_num_rows(p_matrix.m_num_rows), m_num_cols(p_matrix.m_num_cols),
        m_total(m_num_cols * m_num_rows), m_initial(p_matrix.m_initial),
        m_vec((m_num_rows && m_num_cols)
                  ? std::make_unique<data[]>(m_num_rows * m_num_cols)
                  : nullptr)
  {
    if (m_vec)
    {
      std::copy_n(p_matrix.m_vec.get(), m_total, m_vec.get());
    }
  }

  matrix(matrix &&p_matrix)
      : m_num_rows(p_matrix.m_num_rows), m_num_cols(p_matrix.m_num_cols),
        m_total(m_num_cols * m_num_rows), m_initial(p_matrix.m_initial),
        m_vec(std::move(p_matrix.m_vec))
  {
    p_matrix.m_num_rows = 0;
    p_matrix.m_num_cols = 0;
  }

  ~matrix() = default;

  matrix &operator=(const matrix &p_matrix)
  {
    if (this != &p_matrix)
    {
      const auto              total = p_matrix.m_num_rows * p_matrix.m_num_cols;
      std::unique_ptr<data[]> tmp;

      if (total > 0)
      {
        tmp = std::make_unique<data[]>(total);
        std::copy_n(p_matrix.m_vec.get(), total, tmp.get());
      }

      m_initial  = p_matrix.m_initial;
      m_num_rows = p_matrix.m_num_rows;
      m_num_cols = p_matrix.m_num_cols;
      m_total    = total;
      m_vec      = std::move(tmp);
    }
    return *this;
  }

  matrix &operator=(matrix &&p_matrix) noexcept
  {
    if (this != &p_matrix)
    {
      m_vec               = std::move(p_matrix.m_vec);
      m_initial           = p_matrix.m_initial;
      m_num_rows          = p_matrix.m_num_rows;
      m_num_cols          = p_matrix.m_num_cols;
      m_total             = m_num_cols * m_num_rows;
      p_matrix.m_vec      = nullptr;
      p_matrix.m_num_cols = 0;
      p_matrix.m_num_rows = 0;
      p_matrix.m_total    = 0;
    }
    return *this;
  }

  bool operator==(const matrix &p_matrix) const
  {
    if ((get_num_rows() != p_matrix.get_num_rows())
        || (get_num_cols() != p_matrix.get_num_cols()))
    {
      return false;
    }
    for (index _row = 0; _row < p_matrix.get_num_rows(); ++_row)
    {
      for (index _col = 0; _col < p_matrix.get_num_cols(); ++_col)
      {
        if (*get_cell(_row, _col) != *p_matrix.get_cell(_row, _col))
        {
          return false;
        }
      }
    }
    return true;
  }

  friend std::ostream &operator<<(std::ostream &p_out, const matrix &p_matrix)
  {
    if (!p_matrix.m_vec)
    {
      return p_out;
    }

    p_out << '\n';
    for (index _row = 0; _row < p_matrix.get_num_rows(); ++_row)
    {
      for (index _col = 0; _col < p_matrix.get_num_cols(); ++_col)
      {
        p_out << *p_matrix.get_cell(_row, _col) << ' ';
      }
      p_out << '\n';
    }
    return p_out;
  }

  static std::optional<matrix> create(index p_num_rows, index p_num_cols,
                                      data p_initial) noexcept
  {
    try
    {
      return {matrix{p_num_rows, p_num_cols, p_initial}};
    }
    catch (...)
    {
    }
    return std::nullopt;
  }

  data &operator()(index p_row, index p_col)
  {
    if (!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))
    {
      throw std::out_of_range(
          "(!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))");
    }
    return m_vec[(p_row * m_num_cols) + p_col];
  }

  const data &operator()(index p_row, index p_col) const
  {
    if (!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))
    {
      throw std::out_of_range(
          "(!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))");
    }
    return m_vec[(p_row * m_num_cols) + p_col];
  }

  std::optional<std::reference_wrapper<const t_data>>
  get_cell(index p_row, index p_col) const
  {
    if (!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))
    {
      return std::nullopt;
    }
    return {m_vec[(p_row * m_num_cols) + p_col]};
  }

  bool set_cell(index p_row, index p_col, const data &p_data)
  {
    if (!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))
    {
      return false;
    }
    m_vec[(p_row * m_num_cols) + p_col] = p_data;
    return true;
  }

  bool set_cell(index p_row, index p_col, data &&p_data)
  {
    if (!m_vec || (p_row >= m_num_rows) || (p_col >= m_num_cols))
    {
      return false;
    }
    m_vec[(p_row * m_num_cols) + p_col] = std::move(p_data);
    return true;
  }

  inline index get_num_rows() const
  {
    return m_num_rows;
  }

  inline index get_num_cols() const
  {
    return m_num_cols;
  }

  void reset()
  {
    if (!m_vec)
    {
      return;
    }
    std::fill_n(m_vec.get(), m_total, m_initial);
  }

  // private:
  matrix(index p_num_rows, index p_num_cols, data p_initial)
      : m_num_rows(p_num_rows), m_num_cols(p_num_cols),
        m_total(m_num_cols * m_num_rows), m_initial(p_initial),
        m_vec((m_num_rows && m_num_cols)
                  ? std::make_unique<data[]>(m_num_rows * m_num_cols)
                  : nullptr)
  {
    reset();
  }

private:
  index                   m_num_rows{0};
  index                   m_num_cols{0};
  index                   m_total{0};
  data                    m_initial;
  std::unique_ptr<data[]> m_vec;
};

} // namespace tnct::container::dat

#endif
