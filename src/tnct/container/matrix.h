/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_MATRIX_H
#define TNCT_CONTAINER_MATRIX_H

#include <cstring>
#include <memory>

namespace tnct::container {

template <std::integral t_int, typename t_data> struct matrix {
  using index = t_int;
  using data = t_data;

  matrix() = default;
  matrix(const matrix &p_matrix)
      : m_num_rows(p_matrix.m_num_rows), m_num_cols(p_matrix.m_num_cols),
        m_initial(p_matrix.m_initial),
        m_vec(new t_data[m_num_cols * m_num_rows]) {
    std::memcpy(m_vec.get(), p_matrix.m_vec.get(), m_num_rows * m_num_cols);
  }

  matrix(matrix &&p_matrix)
      : m_num_rows(p_matrix.m_num_rows), m_num_cols(p_matrix.m_num_cols),
        m_initial(p_matrix.m_initial), m_vec(std::move(p_matrix.m_vec)) {}

  matrix(t_int p_num_rows, t_int p_num_cols, t_data p_initial)
      : m_num_rows(p_num_rows), m_num_cols(p_num_cols), m_initial(p_initial),
        m_vec(new t_data[m_num_cols * m_num_rows]) {
    reset();
  }

  ~matrix() = default;

  matrix &operator=(const matrix &p_matrix) {
    if (this != &p_matrix) {
      m_initial = p_matrix.m_initial;
      m_num_rows = p_matrix.m_num_rows;
      m_num_cols = p_matrix.m_num_cols;
      m_vec = std::unique_ptr<t_data>(new t_data[m_num_cols * m_num_rows]);
      std::memcpy(m_vec.get(), p_matrix.m_vec.get(), m_num_rows * m_num_cols);
    }
    return *this;
  }

  matrix &operator=(matrix &&p_matrix) {
    if (this != &p_matrix) {
      m_vec = std::move(p_matrix.m_vec);
      m_initial = p_matrix.m_initial;
      m_num_rows = p_matrix.m_num_rows;
      m_num_cols = p_matrix.m_num_cols;
    }
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &p_out, const matrix &p_matrix) {
    if (!p_matrix.m_vec) {
      return p_out;
    }

    p_out << '\n';
    for (t_int _row = 0; _row < p_matrix.get_num_rows(); ++_row) {
      for (t_int _col = 0; _col < p_matrix.get_num_cols(); ++_col) {
        p_out << p_matrix(_row, _col) << ' ';
      }
      p_out << '\n';
    }
    return p_out;
  }

  inline t_data &operator()(t_int p_row, t_int p_col) {
    if (!m_vec) {
      throw std::runtime_error("matrix without rows or cols");
    }
    return m_vec.get()[(p_row * m_num_cols) + p_col];
  }

  inline const t_data &operator()(t_int p_row, t_int p_col) const {
    if (!m_vec) {
      throw std::runtime_error("matrix without rows or cols");
    }
    return m_vec.get()[(p_row * m_num_cols) + p_col];
  }

  inline t_int get_num_rows() const { return m_num_rows; }
  inline t_int get_num_cols() const { return m_num_cols; }

  void reset() {
    for (t_int _row = 0; _row < get_num_rows(); ++_row) {
      for (t_int _col = 0; _col < get_num_cols(); ++_col) {
        (*this)(_row, _col) = m_initial;
      }
    }
  }

private:
  t_int m_num_rows{0};
  t_int m_num_cols{0};

  t_data m_initial;
  std::unique_ptr<t_data> m_vec;
};

} // namespace tnct::container

#endif
