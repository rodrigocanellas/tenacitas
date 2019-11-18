#ifndef CAPEMISA_TEST_AUTOMATION_BUSINESS_GENERATE_INSERT_H
#define CAPEMISA_TEST_AUTOMATION_BUSINESS_GENERATE_INSERT_H

#include <sql/business/insertion_configuration.h>
#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/database.h>
#include <sql/entities/databases.h>
#include <sql/entities/host.h>
#include <sql/entities/hosts.h>
#include <sql/entities/name.h>
#include <sql/entities/server.h>
#include <sql/entities/servers.h>
#include <sql/entities/table.h>
#include <sql/entities/tables.h>

#include <sstream>
#include <string>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;

struct generate_insert
{
  std::string operator()(const hosts& p_hosts,
                         const num_lines_in_tables& p_num_lines_in_tables)
  {
    std::stringstream _stream;

    num_lines_in_tables::const_iterator _end_num_lines_in_tables =
      p_num_lines_in_tables.end();
    for (num_lines_in_tables::const_iterator _ite_num_lines_in_tables =
           p_num_lines_in_tables.begin();
         _ite_num_lines_in_tables != _end_num_lines_in_tables;
         ++_ite_num_lines_in_tables) {
      ptr<table> _table = find_table(p_hosts, _ite_num_lines_in_tables->first);

      if (_table == nullptr) {
        return std::string("");
      }

      _stream << generate_insert_for_table(_table.get(),
                                           _ite_num_lines_in_tables->second);
    }

    return _stream.str();
  }

private:
  std::string generate_insert_for_table(table* p_table, uint16_t p_num_lines)
  {
    std::stringstream _stream;

    uint16_t _num_fks = p_table->get_num_fks();
    for (uint16_t _count_fk = 0; _count_fk < _num_fks; ++_count_fk) {
      _stream << generate_insert_for_table(
                   p_table->get_foregign_key(_count_fk)->get_pk()->get_table(),
                   1)
              << std::endl;
    }

    return _stream.str();
  }

  ptr<table> find_table(const hosts& p_hosts,
                        const table_definition& p_table_definition) const
  {
    const host_name& _host_name = p_table_definition.m_host_name;
    ptr<host> _host =
      p_hosts.find([&_host_name](const ptr<host>& p_host) -> bool {
        return p_host->get_name() == _host_name;
      });

    if (_host == nullptr) {
      return ptr<table>();
    }

    ptr<server> _server = _host->find(p_table_definition.m_server_name);
    if (_server == nullptr) {
      return ptr<table>();
    }

    ptr<database> _database = _server->find(p_table_definition.m_database_name);
    if (_database == nullptr) {
      return ptr<table>();
    }

    ptr<table> _table = _database->find(p_table_definition.m_table_name);
    return _table;
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // GENERATE_INSERT_H
