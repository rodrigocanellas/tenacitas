#ifndef TENACITAS_LOGGER_BUS_FILE_MEDIA_H
#define TENACITAS_LOGGER_BUS_FILE_MEDIA_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

#include <logger/business/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace business {

///
/// \brief The file_media struct implements a \p t_media type to be used in a
/// tenacitas::logger::business::log class, allowing log messages to be written to a
/// file
///
/// \tparam t_controller is a class that will help controlling how the messages
/// will be written, how the files will be created, among other things. It must
/// implement:
/// - move constructor
/// - move assignment
/// - \code void remove() \endcode which will control when a file must be
/// removed
/// - \code void rename(const std::string &) \endcode which will rename a file
/// when no more log messages can be written to the file
/// - \code bool renew(uint32_t) \endcode which will return \p true if the file
/// must be renewed, i.e., if no more messages shall be written to the file, and
/// a new one must be created. To help making the decision, the file size will
/// be passed to the method, in a \p uint32_t value
/// - \code std::string name() \endcode which will provide a name for the file
///
/// \attention There is a default controller created, in file \p
/// file_controller.h
///
template<typename t_controller>
struct file_media
{
  public:
    ///
    /// \brief file_media constructor
    /// \param p_controller the controller of the file, as described in the
    /// class documentation above
    ///
    explicit inline file_media(t_controller&& p_controller = t_controller())
      : m_controller(std::move(p_controller))
      , m_file_name(m_controller.name())
      , m_stream(std::make_shared<std::ofstream>(m_file_name,
                                                 std::ios::app | std::ios::ate))
    {
        cerr_debug(this, " calling remove on ", &m_controller);
        m_controller.remove();
    }

    ///
    /// \brief file_media move constructor
    /// \param p_file_media \p file_media to be moved
    ///
    inline file_media(file_media&& p_file_media) noexcept
      : m_controller(std::move(p_file_media.m_controller))
      , m_file_name(std::move(p_file_media.m_file_name))
      , m_stream(p_file_media.m_stream)
    {
        cerr_debug(this, " moving from ", &p_file_media);
        m_controller.remove();
    }

    file_media(const file_media&) = delete;

    ///
    /// \brief operator = move assignment
    /// \param p_file_media \p file_media to be moved
    /// \return
    ///
    inline file_media& operator=(file_media&& p_file_media) = delete;

    file_media& operator=(const file_media&) = delete;

    ///
    /// \brief ~file_media destructor
    ///
    ~file_media()
    {
        if (!m_file_name.empty()) {
            if (m_stream->is_open()) {
                m_stream->flush();
                m_stream->close();
            }
            m_controller.rename(m_file_name);
        }
    }

    ///
    /// \brief operator () writes a log message to the file
    /// \param p_str the log message
    ///
    void operator()(std::string&& p_str)
    {
        if (m_controller.renew(static_cast<uint32_t>(m_stream->tellp()))) {
            std::string _old_file_name = m_file_name;
            m_file_name = m_controller.name();
            if (m_stream->is_open()) {
                m_stream->flush();
                m_stream->close();
            }
            m_stream = std::make_shared<std::ofstream>(m_file_name);
            m_controller.rename(_old_file_name);
        }
        cerr_debug(this, " writing '", p_str, "' to '", m_file_name, "'");
        (*m_stream) << p_str;
        m_stream->flush();
    }

    ///
    /// \brief type returns a string identifying the media type
    /// \return "file"
    ///
    inline static std::string type() { return "file"; }

  private:
    ///
    /// \brief m_controller helps controlling how the messages will be written,
    /// how the files will be created, among other things. Please, read the \p
    /// file_media class documentation just above the class declaration
    ///
    t_controller m_controller;

    ///
    /// \brief m_file_name the name of the current file
    ///
    std::string m_file_name;

    ///
    /// \brief m_stream is a pointer to the file
    ///
    std::shared_ptr<std::ofstream> m_stream;
};

} // namespace business
} // namespace logger
} // namespace tenacitas

#endif
