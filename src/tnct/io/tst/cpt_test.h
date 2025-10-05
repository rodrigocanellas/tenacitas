/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_IO_TST_CPT_TEST_H
#define TNCT_IO_TST_CPT_TEST_H

#include <cstdint>
#include <expected>
#include <optional>

#include "tnct/io/cpt/reader.h"
#include "tnct/io/cpt/writer.h"
#include "tnct/program/options.h"

using namespace tnct;

namespace tnct::io::tst
{

enum class error : std::int8_t
{
  ERR_0,
  ERR_1
};

std::ostream &operator<<(std::ostream &p_out, error)
{
  return p_out;
}

struct cpt_reader_000
{
  static std::string desc()
  {
    return "Should satisfy Checking 'cpt::reader'";
  }

  bool operator()(const program::options &)
  {
    static_assert(io::cpt::reader<cpt_reader_000, error>,
                  "cpt_reader_000 should satisfy 'cpt::reader'");
    return true;
  }

  std::expected<std::size_t, error> read(std::uint8_t *, std::size_t)
  {
    return 1;
  }
};

struct cpt_reader_001
{
  static std::string desc()
  {
    return "Should satisfy Checking 'cpt::reader'";
  }

  bool operator()(const program::options &)
  {
    static_assert(io::cpt::reader<cpt_reader_001, error>,
                  "cpt_reader_001 should satisfy 'cpt::reader'");

    return true;
  }

  std::expected<std::size_t, error> read(std::uint8_t *, std::size_t)
  {
    return 1;
  }
};

struct cpt_reader_002
{
  static std::string desc()
  {
    return "Should not satisfy Checking 'cpt::reader'";
  }

  bool operator()(const program::options &)
  {
    static_assert(!io::cpt::reader<cpt_reader_002, error>,
                  "cpt_reader_002 should not satisfy 'cpt::reader'");

    return true;
  }

  std::expected<std::size_t, error> read(const std::uint8_t *, std::size_t)
  {
    return std::unexpected{error::ERR_0};
  }
};

struct cpt_reader_003
{
  static std::string desc()
  {
    return "Should not satisfy Checking 'cpt::reader'";
  }

  bool operator()(const program::options &)
  {
    static_assert(!io::cpt::reader<cpt_reader_003, error>,
                  "cpt_reader_003 should not satisfy 'cpt::reader'");

    return true;
  }

  std::expected<std::size_t, error> read(std::uint8_t *, std::uint16_t)
  {
    return std::unexpected{error::ERR_1};
  }
};

struct cpt_reader_004
{
  static std::string desc()
  {
    return "Should not satisfy Checking 'cpt::reader'";
  }

  bool operator()(const program::options &)
  {
    static_assert(!io::cpt::reader<cpt_reader_004, error>,
                  "cpt_reader_004 should not satisfy 'cpt::reader'");

    return true;
  }

  std::size_t read(std::uint8_t *, std::uint16_t)
  {
    return 0;
  }
};

struct cpt_writer_000
{
  static std::string desc()
  {
    return "Should satisfy 'cpt::writer";
  }

  bool operator()(const program::options &)
  {

    static_assert(io::cpt::writer<cpt_writer_000, error>,
                  "Should satisfy 'cpt::writer");

    return true;
  }

  std::optional<error> write(const std::uint8_t *, std::size_t)
  {
    return std::nullopt;
  }
};

struct cpt_writer_001
{
  static std::string desc()
  {
    return "Should not satisfy 'cpt::writer";
  }

  bool operator()(const program::options &)
  {

    static_assert(!io::cpt::writer<cpt_writer_001, error>,
                  "Should not satisfy 'cpt::writer");

    return true;
  }

  std::optional<error> write(std::uint8_t *, std::size_t)
  {
    return std::nullopt;
  }
};

struct cpt_writer_002
{
  static std::string desc()
  {
    return "Should not satisfy 'cpt::writer";
  }

  bool operator()(const program::options &)
  {

    static_assert(!io::cpt::writer<cpt_writer_002, error>,
                  "Should not satisfy 'cpt::writer");

    return true;
  }

  std::optional<error> write(const std::uint8_t *, int)
  {
    return std::nullopt;
  }
};

struct cpt_writer_003
{
  static std::string desc()
  {
    return "Should not satisfy 'cpt::writer";
  }

  bool operator()(const program::options &)
  {

    static_assert(!io::cpt::writer<cpt_writer_003, error>,
                  "Should not satisfy 'cpt::writer");

    return true;
  }

  error write(const std::uint8_t *, std::size_t)
  {
    return error::ERR_0;
  }
};

} // namespace tnct::io::tst

#endif
