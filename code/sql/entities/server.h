#ifndef CAPEMISA_SQL_ENTITIES_SERVER_H
#define CAPEMISA_SQL_ENTITIES_SERVER_H

#include <iostream>

#include <sql/entities/database.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>

namespace capemisa {
namespace sql {
namespace entities {

using namespace generic;

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

  inline explicit server(const name& p_name, const host* p_host)
    : m_name(p_name)
    , m_host(p_host)
  {}

  inline const name& get_name() const { return m_name; }

  ptr<database> find(const name& p_database_name);

  inline uint16_t get_num_databases() const
  {
    return m_databases.get_size<uint16_t>();
  }

  inline ptr<database> get_database(uint16_t p_index)
  {
    return m_databases[p_index];
  }

  ptr<database> add_database(const name& p_database_name);

  const host& get_host() const;

private:
  typedef collection<database> databases;

private:
  name m_name;
  const host* m_host;
  databases m_databases;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // SERVER_H
