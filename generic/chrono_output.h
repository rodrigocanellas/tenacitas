/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_CHRONO_OUTPUT_H
#define TENACITAS_LIB_TRAITS_CHRONO_OUTPUT_H

#include <chrono>
#include <iostream>

#include <tenacitas.lib/traits/chrono_convertible.h>

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::nanoseconds p_time) {
  p_out << p_time.count() << " ns";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::microseconds p_time) {
  p_out << p_time.count() << " us";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::milliseconds p_time) {
  p_out << p_time.count() << " ms";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::seconds p_time) {
  p_out << p_time.count() << " s";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::minutes p_time) {
  p_out << p_time.count() << " min";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::hours p_time) {
  p_out << p_time.count() << " hs";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::days p_time) {
  p_out << p_time.count() << " days";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::months p_time) {
  p_out << p_time.count() << " m";
  return p_out;
}

static inline std::ostream &operator<<(std::ostream &p_out,
                                       std::chrono::years p_time) {
  p_out << p_time.count() << " y";
  return p_out;
}

#endif
