
#include <logger/business/internal/file_controller.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace business {

// ----------------------------------------------------------------------------
std::string
file_controller::name()
{
    std::stringstream _stream;
    update_last();

    static const uint16_t _max_str_length =
      string::business::max_str_length<pid_t>();
    _stream << m_path << "/" << m_base_name << "_" << std::right
            << std::setfill('0') << std::setw(_max_str_length) << m_pid << "_"
            << m_last << ".log";
    return _stream.str();
}

////
///----------------------------------------------------------------------------
// bool
// file_controller::exclude()
//{
//    cerr_debug(this, " m_retention = ", m_retention.count());
//    tenacitas::logger::business::configure_cerr_log();
//    DIR* _dir = nullptr;
//    struct dirent* _ent = nullptr;
//    _dir = opendir(m_path.c_str());
//    if (_dir != nullptr) {
//        _ent = readdir(_dir);
//        while (_ent != nullptr) {
//            struct stat _stat;
//            if ((stat(_ent->d_name, &_stat) >= 0) &&
//                // is a regular file
//                ((_stat.st_mode & S_IFMT) == S_IFREG) &&
//                // it is writable
//                (_stat.st_mode & S_IWUSR) &&
//                // is old enough
//                ((time(nullptr) - _stat.st_mtim.tv_sec) >
//                 m_retention.count()) &&
//                // name matches the pattern
//                (match(_ent->d_name))) {
//                cerr_info("removing ", _ent->d_name);
//                std::remove(_ent->d_name);
//            }
//            _ent = readdir(_dir);
//        }
//        closedir(_dir);
//    }

//    return true;
//}

// ----------------------------------------------------------------------------
bool
file_controller::deleter::operator()()
{
    tenacitas::logger::business::configure_cerr_log();
    DIR* _dir = nullptr;
    struct dirent* _ent = nullptr;
    _dir = opendir(m_path.c_str());
    if (_dir != nullptr) {
        _ent = readdir(_dir);
        while (_ent != nullptr) {
            struct stat _stat;
            if ((stat(_ent->d_name, &_stat) >= 0) &&
                // is a regular file
                ((_stat.st_mode & S_IFMT) == S_IFREG) &&
                // it is writable
                (_stat.st_mode & S_IWUSR) &&
                // is old enough
                ((time(nullptr) - _stat.st_mtim.tv_sec) >
                 m_retention.count()) &&
                // name matches the pattern
                (match(_ent->d_name))) {
                std::remove(_ent->d_name);
            }
            _ent = readdir(_dir);
        }
        closedir(_dir);
    }

    return true;
}

} // namespace business
} // namespace logger
} // namespace tenacitas
