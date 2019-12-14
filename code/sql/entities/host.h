#ifndef CAPEMISA_SQL_ENTITIES_HOST_H
#define CAPEMISA_SQL_ENTITIES_HOST_H

#include <algorithm>
#include <iostream>

#include <sql/entities/ip.h>
#include <sql/entities/server.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace entities {

using namespace generic;

struct host
{

  friend std::ostream& operator<<(std::ostream& p_out, const host& p_host);

  host() = delete;
  host(const host&) = delete;
  host(host&&) = delete;
  host& operator=(const host&) = delete;
  host& operator=(host&&) = delete;
  ~host() = default;

  host(const name& p_name, const ip& p_ip)
    : m_name(p_name)
    , m_ip(p_ip)
  {}

  inline uint16_t get_num_servers() const
  {
    return m_servers.get_size<uint16_t>();
  }

  inline ptr<server> get_server(uint16_t p_index) { return m_servers[p_index]; }

  inline ptr<server> find(const name& p_server_name)
  {
    return m_servers.find(p_server_name);
  }

  ptr<server> add_server(const name& p_server_name);

  inline const name& get_name() const { return m_name; }

  inline const ip& get_ip() const { return m_ip; }

private:
  typedef collection<server> servers;

private:
  name m_name;
  ip m_ip;
  servers m_servers;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // HOST_H
