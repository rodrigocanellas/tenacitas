#ifndef TENACITAS_BUSINESS_LOGGER_FILE_CONTROLLER_H
#define TENACITAS_BUSINESS_LOGGER_FILE_CONTROLLER_H

#include <chrono>
#include <cstdint>
#include <dirent.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <business/calendar/conversions.h>
#include <business/calendar/epoch.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/log.h>
#include <business/string/max_str_length.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace business {
/// \brief namespace of the group of classes
namespace logger {

///
/// \brief The file_controller struct controls the creation, nomination,
/// closing and cleaning of log files
///
/// \details A file name has this format:
/// 'base-name'_'pid'_'epoch-millisecs'.log, like
/// test008_0000004621_1546023437240600.log
///
/// When it is closed, possibly because it reached a user defined maximum size,
/// it becomes 'base-name'_'pid'_'epoch-millisecs'.log.'closed-extension', like
/// test008_0000004621_1546023437240600.log.closed
///
/// After a user defined amount of time, the files that are closed will be
/// deleted
///
struct file_controller
{
    ///
    /// \brief file_controller constructor
    /// \param p_path determines where the log files will be created
    /// \param p_base_name is used in the begining of the file name
    /// \param p_max_file_size maximum allowed size, in bytes, that the log
    /// file can reach, before another is created
    /// \param p_retention amount of time that a closed file will exist before
    /// deletion
    /// \param p_closed_extension the extension added to the log file when it
    /// was closed, possibly because it reached \p p_max_file_size size
    ///
    inline explicit file_controller(
      const std::string& p_path = ".",
      const std::string& p_base_name = "log",
      uint32_t p_max_file_size = 10 * 1024 * 1024,
      std::chrono::minutes p_retention =
        std::chrono::minutes(business::calendar::day2min(2)),
      const std::string& p_closed_extension = "closed")
      : m_path(std::move(p_path))
      , m_base_name(std::move(p_base_name))
      , m_closed_extension(p_closed_extension)
      , m_last(0)
      , m_pid(getpid())
      , m_max_file_size(p_max_file_size)
      , m_deleter(m_path,
                  m_base_name,
                  m_closed_extension,
                  std::chrono::seconds(
                    business::calendar::min2sec(p_retention.count())))
      , m_sleeping_loop(
          std::chrono::milliseconds(
            business::calendar::min2mil(p_retention.count())),
          [this]() -> business::concurrent::result { return this->m_deleter(); },
          std::chrono::milliseconds(business::calendar::min2mil(20)))
    {}

    file_controller() = delete;

    file_controller(const file_controller&) = delete;

    file_controller(file_controller&& p_controller) noexcept
      : m_path(std::move(p_controller.m_path))
      , m_base_name(std::move(p_controller.m_base_name))
      , m_closed_extension(std::move(p_controller.m_closed_extension))
      , m_last(p_controller.m_last)
      , m_pid(p_controller.m_pid)
      , m_max_file_size(p_controller.m_max_file_size)
      , m_deleter(std::move(p_controller.m_deleter))
      , m_sleeping_loop(p_controller.m_sleeping_loop.get_interval(),
                        [this]() -> business::concurrent::result { return this->m_deleter(); },
                        p_controller.m_sleeping_loop.get_timeout())
    {}

    ~file_controller() { m_sleeping_loop.stop(); }

    file_controller& operator=(const file_controller&) = delete;
    file_controller& operator=(file_controller&& p_ctrl) noexcept = delete;

    ///
    /// \brief name retrieves the name of the log file
    /// \return the value of the log file name
    ///
    std::string name();

    ///
    /// \brief rename renames the log file, apppending the user defined closed
    /// extension
    /// \param p_file_name the original file name
    ///
    inline void rename(const std::string& p_file_name) const
    {
        std::rename(
          p_file_name.c_str(),
          std::string(p_file_name + "." + m_closed_extension).c_str());
    }

    ///
    /// \brief renew determines if the file needs to be closed and a new one
    /// created
    /// \param p_file_size the current file size, in bytes
    /// \return \p true if it needs to be renewed, \p false otherwise
    ///
    inline bool renew(uint32_t p_file_size) const
    {
        return (p_file_size >= static_cast<uint32_t>(0.95 * m_max_file_size));
    }

    ///
    /// \brief remove initiates a asynchronous loop, time controlled, that will
    /// check which files can be deleted
    ///
    inline void remove() { m_sleeping_loop.run(); }

  private:
    ///
    /// \brief sleeping_loop_t an alias for the sleeping loop used
    ///
    typedef business::concurrent::sleeping_loop_t<void, log> sleeping_loop;

    struct deleter
    {
        deleter() = delete;

        deleter(const deleter&) = default;
        deleter(deleter&&) noexcept = default;

        deleter& operator=(const deleter&) = default;
        deleter& operator=(deleter&&) /*noexcept*/ = default;

        ~deleter() = default;

        deleter(const std::string& p_path,
                const std::string& p_base_name,
                const std::string& p_closed_extension,
                std::chrono::seconds p_retention)
          : m_path(p_path)
          , m_base_name(p_base_name)
          , m_closed_extension(p_closed_extension)
          , m_retention(p_retention)
        {}

        business::concurrent::result operator()();

      private:
        ///
        /// \brief match checks if a file name matchs the criteria for the file
        /// to be examined for deletion
        /// \param p_file_name the name of the file to be verified
        /// \return \p true if matchs, \p false otherwise
        ///
        inline bool match(const std::string& p_file_name)
        {
            return (
              (p_file_name.find(m_base_name) != std::string::npos) &&
              (p_file_name.find(m_closed_extension) != std::string::npos));
        }

      private:
        ///
        /// \brief m_path path of the log files
        ///
        std::string m_path = ".";

        ///
        /// \brief m_base_name initial string in file name
        ///
        std::string m_base_name = "log";

        ///
        /// \brief m_closed_extension extension appended to the file name, when
        /// the file is closed
        ///
        std::string m_closed_extension = "closed";

        ///
        /// \brief m_retention amount of time that the file will exist before
        /// deleted
        ///
        std::chrono::seconds m_retention = std::chrono::seconds(15 * 60);
    };

  private:
    ///
    /// \brief update_last tries to prevent two file with the same name, by
    /// changing the epoch part of the name
    ///
    void update_last()
    {
        uint64_t _id = business::calendar::epoch::microsecs();
        if (_id == m_last) {
            m_last = _id + 1;
        } else {
            m_last = _id;
        }
    }

    ///
    /// \brief match checks if a file name matchs the criteria for the file
    /// to be examined for deletion
    /// \param p_file_name the name of the file to be verified
    /// \return \p true if matchs, \p false otherwise
    ///
    inline bool match(const std::string& p_file_name)
    {
        return ((p_file_name.find(m_base_name) != std::string::npos) &&
                (p_file_name.find(m_closed_extension) != std::string::npos));
    }

  private:
    ///
    /// \brief m_path path of the log files
    ///
    std::string m_path;

    ///
    /// \brief m_base_name initial string in file name
    ///
    std::string m_base_name;

    ///
    /// \brief m_closed_extension extension appended to the file name, when
    /// the file is closed
    ///
    std::string m_closed_extension;

    ///
    /// \brief m_last last epoch used in the file name
    ///
    uint64_t m_last;

    ///
    /// \brief m_pid process identifier
    ///
    pid_t m_pid;

    ///
    /// \brief m_max_file_size maximum size that a file can reach, before its
    /// closed
    ///
    uint32_t m_max_file_size;

    deleter m_deleter;

    ///
    /// \brief m_sleeping_loop asynchronous time controlled loop that will check
    /// if files should be removed
    ///
    sleeping_loop m_sleeping_loop;
};

} // namespace logger
} // namespace business
} // namespace tenacitas

#endif // default_controller_H
