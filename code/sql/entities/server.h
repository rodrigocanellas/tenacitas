#ifndef CAPEMISA_SQL_ENTITIES_SERVER_H
#define CAPEMISA_SQL_ENTITIES_SERVER_H

#include <iostream>

#include <sql/entities/host.h>
#include <sql/generic/collection.h>
#include <sql/entities/database.h>
#include <sql/generic/name.h>

namespace capemisa {
namespace sql {
namespace entities {

struct host;

struct server
{

  friend std::ostream& operator<<(std::ostream& p_out, const server& p_server);

  server() = delete;
  server(const server&) = delete;
  server(server&&) = delete;
  server& operator=(const server&) = delete;
  server& operator=(server&&) = delete;
  ~server() = default;

  inline explicit server(const generic::name& p_name, const host* p_host)
    : m_name(p_name)
    , m_host(p_host)
  {}

  inline const generic::name& get_name() const { return m_name; }

  generic::ptr<database> find(const generic::name& p_database_name);

  inline uint16_t get_num_databases() const
  {
    return m_databases.get_size<uint16_t>();
  }

  inline generic::ptr<database> get_database(uint16_t p_index)
  {
    return m_databases[p_index];
  }

  generic::ptr<database> add_database(const generic::name& p_database_name);

  const host& get_host() const;

  private:
  typedef generic::collection<database> databases;

  private:
    generic::name m_name;
  const host * m_host;
  databases m_databases;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // SERVER_H
