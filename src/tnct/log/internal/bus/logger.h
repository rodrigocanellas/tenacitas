/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_INTERNAL_LOGGER_H
#define TNCT_LOG_INTERNAL_LOGGER_H

#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <utility>

#include "tnct/log/bus/line_header_formater.h"
#include "tnct/log/cpt/writer.h"
#include "tnct/log/dat/level.h"

namespace tnct::log::internal::bus
{

/// \brief Thread safe writing of log messages
///
/// A log message has the format:
/// log-level|timestamp|thread-id|function-name|line-number|contents
///
/// \details \p log-level is TST, TRA, DEB, INF, WAR, ERR or FAT; \p timestamp
/// is a EPOCH number in microsecs; \p thread-id is the id of the thread that is
/// logging; \p funcition-name is the name of the function \p line-number is the
/// number of the line that is logging; contents are the log message specific
/// contents
template <log::cpt::writer t_log_writer>
class logger
{

public:
  /// \brief Default contructor
  ///
  /// \p p_writer  Responsible for actually writing the log message
  logger(t_log_writer       &&p_log_writer,
         line_header_formater p_line_header_formater)
      : m_writer(std::move(p_log_writer)),
        m_line_header_formater(p_line_header_formater)
  {
  }

  logger()
      : m_writer(t_log_writer()),
        m_line_header_formater(default_line_header_formater)
  {
  }

  logger(line_header_formater p_line_header_formater)
      : m_writer(t_log_writer()), m_line_header_formater(p_line_header_formater)
  {
  }

  logger(t_log_writer &&p_log_writer)
      : m_writer(std::move(p_log_writer)),
        m_line_header_formater(default_line_header_formater)
  {
  }

  /// \brief Copy constructor not allowed
  logger(const logger &) = delete;

  /// \brief Move constructor not allowed
  logger(logger &&) = delete;

  /// \brief Copy assignment not allowed
  logger &operator=(const logger &) = delete;

  /// \brief Move assignment not allowed
  logger &operator=(logger &&) = delete;

  /// \brief New operator not allowed
  void *operator new(size_t) = delete;

  /// \brief Delete operator not allowed
  void operator delete(void *) = delete;

  /// \brief New operator not allowed
  void *operator new[](size_t) = delete;

  /// \brief Delete operator not allowed
  void operator delete[](void *) = delete;

  void set_header_formater(line_header_formater p_line_header_formater)
  {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_line_header_formater = p_line_header_formater;
  }

  /// \brief Defines the separator to be used in the log messages
  /// Default is '|'
  ///
  /// \param p_separator the value of the separator
  inline void set_separator(char p_separator)
  {
    m_separator = p_separator;
  }

  /// \brief Retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  inline char get_separator() const
  {
    return m_separator;
  }

  /// \brief Logs message with \p tst severity
  /// Each parameter must implement the output operator.
  /// The log message will be allways written, and it should only be used in
  /// tests
  void tst(std::string_view     message,
           std::source_location p_location = std::source_location::current())
  {
    write(dat::level::tst, message, p_location);
  }

  /// \brief Logs message with \p tra severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::tra
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_source_location contains data about the source
  ///
  /// \param p_params are the contens of the log message
  inline void
  tra(std::string_view     p_string,
      std::source_location p_source_location = std::source_location::current())
  {
    if (dat::level::tra >= m_level)
    {
      write(dat::level::tra, p_string, p_source_location);
    }
  }

  /// \brief Logs message with \p deb severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::deb or \p level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_source_location contains data about the source
  ///
  /// \param p_params are the contens of the log message
  inline void
  deb(std::string_view     p_string,
      std::source_location p_source_location = std::source_location::current())
  {
    if (dat::level::deb >= m_level)
    {
      write(dat::level::deb, p_string, p_source_location);
    }
  }

  /// \brief Logs message with \p inf severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::inf, \p level::debug or \p level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_source_location contains data about the source
  ///
  /// \param p_params are the contens of the log message
  inline void
  inf(std::string_view     p_string,
      std::source_location p_source_location = std::source_location::current())
  {
    if (dat::level::inf >= m_level)
    {
      write(dat::level::inf, p_string, p_source_location);
    }
  }

  /// \brief Logs message with \p war severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::war, \p level::info, \p level::debug or \p
  /// level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_source_location contains data about the source
  ///
  /// \param p_params are the contens of the log message
  inline void
  war(std::string_view     p_string,
      std::source_location p_source_location = std::source_location::current())
  {
    if (dat::level::war >= m_level)
    {
      write(dat::level::war, p_string, p_source_location);
    }
  }

  /// \brief Logs message with \p err severity
  /// Each parameter must implement the output operator.
  /// \p level::err messages are allways logged
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_source_location contains data about the source
  ///
  /// \param p_params are the contens of the log message
  inline void
  err(std::string_view     p_string,
      std::source_location p_source_location = std::source_location::current())
  {
    write(dat::level::err, p_string, p_source_location);
  }

  /// \brief Logs message with \p fat severity
  /// Each parameter must implement the output operator.
  /// \p level::fat messages are allways logged
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_source_location contains data about the source
  ///
  /// \param p_params are the contens of the log message
  inline void
  fat(std::string_view     p_string,
      std::source_location p_source_location = std::source_location::current())
  {
    write(dat::level::fat, p_string, p_source_location);
  }

  inline void set_tra()
  {
    m_level = dat::level::tra;
  }

  inline void set_deb()
  {
    m_level = dat::level::deb;
  }

  inline void set_inf()
  {
    m_level = dat::level::inf;
  }

  inline void set_war()
  {
    m_level = dat::level::war;
  }

  inline bool can_tra() const
  {
    return m_level <= dat::level::tra;
  }

  inline bool can_deb() const
  {
    return m_level <= dat::level::deb;
  }
  inline bool can_inf() const
  {
    return m_level <= dat::level::inf;
  }
  inline bool can_war() const
  {
    return m_level <= dat::level::war;
  }

  void save_level()
  {
    m_saved_level = m_level;
  }

  void restore_level()
  {
    m_level = m_saved_level;
  }

private:
  void write(dat::level p_level, std::string_view p_string,
             std::source_location &p_source_location)
  {

    std::stringstream _stream;
    m_line_header_formater(_stream, p_level, p_source_location)
        << p_string << std::endl;

    std::lock_guard<std::mutex> _lock(m_mutex);
    m_writer(_stream.str());
  }

private:
  /// \brief Function that actually writes the log message
  ///
  /// The default implementation writes the \p std::cerr
  t_log_writer m_writer;

  line_header_formater m_line_header_formater;

  /// \brief Allows a thread safe writing to the log writer
  std::mutex m_mutex;

  /// \brief Used to separate parts of the log message
  char m_separator{'|'};

  /// \brief Current log dat::level
  dat::level m_level{dat::level::inf};

  dat::level m_saved_level{dat::level::inf};
};

} // namespace tnct::log::internal::bus

#endif
