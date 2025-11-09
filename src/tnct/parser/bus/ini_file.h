#ifndef TNCT_PARSER_INI_FILE_H
#define TNCT_PARSER_INI_FILE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <fstream>
#include <functional>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <string_view>

#include "tnct/format/bus/fmt.h"
#include "tnct/log/cpt/logger.h"

namespace tnct::parser::bus
{

template <log::cpt::logger t_logger>
struct ini_file
{
  using key        = std::string;
  using value      = std::string;
  using properties = std::map<key, value>;
  using section    = std::string;
  using sections   = std::map<section, properties>;
  using logger     = t_logger;

  ini_file(logger &p_logger) : m_logger(p_logger)
  {
  }

  std::optional<sections> read(std::string_view p_filename) const
  {
    sections _sections;
    try
    {
      std::ifstream _file(p_filename.data());

      if (!_file.is_open())
      {
        m_logger.err(format::bus::fmt("could not open '", p_filename, '\''));
        return std::nullopt;
      }

      section    _section;
      properties _properties;

      static const std::regex _section_regex{
          R"((?:\[)(?:\s*)(.*\b)(?:\s*)(?:\]))"};
      static const std::regex _property_regex{
          R"((?:\s*)(.*)(?:\s*)=(?:\s*)(.*)(?:\s*))"};

      size_t _line_number(0);
      for (std::string _line; std::getline(_file, _line);)
      {
        ++_line_number;
        m_logger.tra(
            format::bus::fmt("line # ", _line_number, " = '", _line, '\''));
        std::smatch _matches;

        std::regex_search(_line, _matches, _section_regex);

        if (_line[0] == '#')
        {
          m_logger.tra(
              format::bus::fmt("line # ", _line_number, " is a comment"));
          continue;
        }

        if (_matches.size() == 2)
        {
          if (!_section.empty())
          {
            m_logger.tra(
                format::bus::fmt("sections not empty: '", _section, '\''));
            _sections.insert({std::move(_section), std::move(_properties)});
            _section    = section(_matches[1]);
            _properties = properties();
          }
          else
          {
            // first section
            m_logger.tra(format::bus::fmt("first section"));
            _section = _matches[1];
            m_logger.tra(
                format::bus::fmt("sections now with: '", _section, '\''));
          }
        }
        else
        {
          std::regex_search(_line, _matches, _property_regex);
          if (_matches.size() == 3)
          {
            _properties.insert({_matches[1].str(), _matches[2].str()});
          }
          else
          {
            m_logger.tra(
                format::bus::fmt("line # ", _line_number, " '", _line,
                                 "' does not contain a section or value"));
          }
        }
      }
      if (!_section.empty())
      {
        _sections.insert({std::move(_section), std::move(_properties)});
      }
    }
    catch (std::exception &_ex)
    {
      m_logger.err(format::bus::fmt("ERROR reading INI file '", p_filename,
                                    "': '", _ex.what(), '\''));
      return std::nullopt;
    }
    return std::optional<sections>(std::move(_sections));
  }

  inline void
  traverse(const sections                                          &p_sections,
           std::function<bool(const section &, const properties &)> p_visit)
  {
    for (const sections::value_type &_pair : p_sections)
    {
      if (!p_visit(_pair.first, _pair.second))
      {
        break;
      }
    }
  }

  inline void traverse(const properties &p_properties,
                       std::function<bool(const key &, const value &)> p_visit)
  {
    for (const properties::value_type &_pair : p_properties)
    {
      if (!p_visit(_pair.first, _pair.second))
      {
        break;
      }
    }
  }

private:
  logger &m_logger;
};

} // namespace tnct::parser::bus

#endif // TNCT_INI_H
