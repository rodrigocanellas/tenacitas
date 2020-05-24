#include <communication/status.h>
/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

const status status::ok(0);
const status status::error_unspecified(1);
const status status::error_connecting(2);
const status status::error_sending(3);
const status status::error_posting(4);
const status status::error_notifying(5);
const status status::error_creating_security(6);
const status status::error_receiving(7);
const status status::error_timeout(8);

} // namespace communication
} // namespace tenacitas
