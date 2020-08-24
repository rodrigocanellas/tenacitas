
#include <logger/file/internal/file_controller.h>

#include <cstring>
#include <iomanip>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace file {

// ----------------------------------------------------------------------------
// std::string file_controller::name() {
//  std::stringstream _stream;
//  update_last();

//  using namespace std::chrono;

//  const auto _duration = std::chrono::milliseconds(m_last);
//  const time_point<system_clock> _time_point(_duration);
//  const time_t _time_t = system_clock::to_time_t(_time_point);

//  const auto _remainder = m_last % 1000;

//  //  static const uint16_t _max_str_length =
//  formater::max_str_length<pid_t>();
//  //  _stream << m_path << "/" << m_base_name << "_" << std::right
//  //          << std::setfill('0') << std::setw(_max_str_length) << m_pid <<
//  "_"
//  //          << m_last << ".log";
//  _stream << m_path << "/" << m_base_name << "-" << number::format_000(m_pid)
//          << "-"
//          << std::put_time(std::localtime(&_time_t), "%Y-%m-%d-%H-%M-%S-")
//          << std::setw(3) << std::setfill('0') << std::left << _remainder
//          << ".log";
//  return _stream.str();
//}

std::string file_controller::name() {
  std::stringstream _stream;
  update_last();

  using namespace std::chrono;

  const auto _duration = std::chrono::microseconds(m_last);
  const time_point<system_clock> _time_point(_duration);
  const time_t _time_t = system_clock::to_time_t(_time_point);

  const auto _remainder = m_last % 1000000;

  //  static const uint16_t _max_str_length = formater::max_str_length<pid_t>();
  //  _stream << m_path << "/" << m_base_name << "_" << std::right
  //          << std::setfill('0') << std::setw(_max_str_length) << m_pid << "_"
  //          << m_last << ".log";
  _stream << m_path << "/" << m_base_name << "-" << number::format_000(m_pid)
          << "-"
          << std::put_time(std::localtime(&_time_t), "%Y-%m-%d-%H-%M-%S-")
          << std::setw(6) << std::setfill('0') << std::left << _remainder
          << ".log";
  return _stream.str();
}

// ----------------------------------------------------------------------------
status::result file_controller::deleter::operator()() {
  m_log.debug(__LINE__, "STARTING CONTROLLER");
#ifdef WIN32
  m_log.debug(__LINE__, "windows!");
  WIN32_FIND_DATAA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  DWORD dwError = 0;

  std::string _pattern(m_path + "/" + m_base_name + "*." + m_closed_extension);

  hFind = FindFirstFileA(_pattern.c_str(), &ffd);

  m_log.debug(__LINE__, "hFind = ", hFind);

  if (INVALID_HANDLE_VALUE == hFind) {
    m_log.error(__LINE__, "could not open ", _pattern);
    return status::ok;
  }

  uint32_t _total = 0;
  uint32_t _deleted = 0;

  // List all the files in the directory with some info about them.
  SYSTEMTIME stUTC, stLocal;
  do {

    m_log.debug(__LINE__, "file = ", ffd.cFileName);

    FILETIME _file_time = ffd.ftLastWriteTime;
    FileTimeToSystemTime(&_file_time, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    std::tm tm;

    tm.tm_sec = stLocal.wSecond;
    tm.tm_min = stLocal.wMinute;
    tm.tm_hour = stLocal.wHour;
    tm.tm_mday = stLocal.wDay;
    tm.tm_mon = stLocal.wMonth - 1;
    tm.tm_year = stLocal.wYear - 1900;
    tm.tm_isdst = -1;

    bool _is_normal = (ffd.dwFileAttributes &
                       (FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE));

    bool _is_read = (ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY);

    std::time_t t = std::mktime(&tm);

    time_t _now = time(nullptr);

    auto _expired = (_now - t) > m_retention.count();
    m_log.debug(__LINE__, ffd.cFileName, " is expired? ",
                (_expired ? "yes" : "no"));

    ++_total;

    if ((_is_normal) && (!_is_read) && (_expired)) {
      std::string _full_path(m_path + "/" + ffd.cFileName);
      m_log.info(__LINE__, _full_path, " WILL BE REMOVED");
      ++_deleted;
      DeleteFileA(_full_path.c_str());
    } else {
      m_log.debug(__LINE__, "file ", ffd.cFileName, " will be not removed");
    }

  } while (FindNextFileA(hFind, &ffd) != 0);

  dwError = GetLastError();
  if (dwError != ERROR_NO_MORE_FILES) {
    m_log.error(__LINE__, dwError);
  }

  FindClose(hFind);

  m_log.info(__LINE__, "################# ", _total, " read, ", _deleted,
             " deleted");

#else
  DIR *_dir = nullptr;
  struct dirent *_ent = nullptr;
  _dir = opendir(m_path.c_str());
  if (_dir != nullptr) {
    m_log.debug(__LINE__, "_dir != null for ", m_path);
    _ent = readdir(_dir);
    while (_ent != nullptr) {
      m_log.debug(__LINE__, "_ent != null for ", _ent->d_name);

      struct stat _stat;

      int _res = stat(_ent->d_name, &_stat);
      bool _stats_retrieved = (_res >= 0);
      m_log.debug(__LINE__, "stats = ", _res);

      bool _is_regular_file = (_stat.st_mode & S_IFMT) == S_IFREG;
      m_log.debug(__LINE__, "_is_regular_file = ", _is_regular_file);

      bool _is_writable = (_stat.st_mode & S_IWUSR);
      m_log.debug(__LINE__, "_is_writable = ", _is_writable);

      bool _is_old_enough =
          ((time(nullptr) - _stat.st_mtime) > m_retention.count());
      m_log.debug(__LINE__, "_is_old_enough = ", _is_old_enough);

      // name matches the pattern
      bool _name_match = (match(_ent->d_name));
      m_log.debug(__LINE__, "_name_match = ", _name_match);

      if (_stats_retrieved && _is_regular_file && _is_writable &&
          _is_old_enough && _name_match) {
        m_log.debug(__LINE__, "removing ", _ent->d_name);
        std::remove(_ent->d_name);
      }
      _ent = readdir(_dir);
    }
    closedir(_dir);
  } else {
    m_log.debug(__LINE__, "_dir == null for ", m_path);
  }
#endif
  return status::ok;
}

} // namespace file
} // namespace logger
} // namespace tenacitas
