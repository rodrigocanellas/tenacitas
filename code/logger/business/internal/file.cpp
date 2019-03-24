#include <logger/business/file.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
namespace business {

// ----------------------------------------------------------------------------
void
configure_file_log(const std::string& p_path,
                   const std::string& p_base_name,
                   uint32_t p_max_file_size,
                   std::chrono::minutes p_retention,
                   const std::string& p_closed_extension)
{

    cerr_debug("creating file_controller_t");
    business::file_controller_t _fc(
      p_path, p_base_name, p_max_file_size, p_retention, p_closed_extension);
    cerr_debug("file_controller_t creaeted: ", &_fc);
    cerr_debug("creating file media ");
    business::file_media_t _fm(std::move(_fc));
    cerr_debug("file media created: ", &_fm);
    business::file_log_t::configure(std::move(_fm));
}

} // namespace business
} // namespace logger
} // namespace tenacitas
