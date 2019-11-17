#include <sql/entities/host.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const host& p_host)
{

  p_out << "{\"name\":\"" << p_host.get_name() << "\", "
        << "\"ip\" : \"" << p_host.get_ip() << "\", "
        << "\"servers\" : " << p_host.m_servers << "}";

  return p_out;
}

ptr<server>
host::add_server(const name& p_server_name)
{
  ptr<server> _server = find(p_server_name);
  if (_server == nullptr) {
    _server = make_ptr<server>(this, p_server_name);
    m_servers.add(_server);
  }
  return _server;
}

} // namespace entities
} // namespace sql
} // namespace capemisa
