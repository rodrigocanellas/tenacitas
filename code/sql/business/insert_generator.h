#ifndef CAPEMISA_TEST_AUTOMATION_BUSINESS_generate_table_insert_H
#define CAPEMISA_TEST_AUTOMATION_BUSINESS_generate_table_insert_H

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/database.h>
#include <sql/entities/databases.h>
#include <sql/entities/host.h>
#include <sql/entities/hosts.h>
#include <sql/entities/insert_definition.h>
#include <sql/entities/inserts_definitions.h>
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

struct insert_generator
{
  std::string operator()(const hosts& p_hosts,
                         const inserts_definitions& p_inserts_definitions)
  {
    std::stringstream _stream;

    inserts_definitions::const_iterator _end = p_inserts_definitions.end();
    inserts_definitions::const_iterator _ite = p_inserts_definitions.begin();
    for (; _ite != _end; ++_ite) {
      _stream << generate_table_insert(p_hosts, *_ite) << std::endl;
    }
    return _stream.str();
  }

private:
  struct column_created
  {
    column_name m_column_name;
    std::string m_value;
  };

  struct foreign_key_created
  {
    typedef std::list<column_created> columns_created;

    ptr<foreign_key> m_foreign_key;
    columns_created m_columns_created;
  };

  typedef std::list<foreign_key_created> foreigners_keys_created;

  std::string generate_table_insert(
    const hosts p_hosts,
    const insert_definition& p_insert_definition)
  {

    ptr<table> _table = find_table(p_hosts, p_insert_definition);

    if (_table == nullptr) {
      return "";
    }

    std::stringstream _stream;

    foreigners_keys_created _foreigners_keys_created;

    _stream << generate_inserts_fks(
      _table, p_insert_definition, _foreigners_keys_created);

    uint16_t _amount_of_rows = p_insert_definition.get_amount_of_rows();

    for (uint16_t _row = 0; _row < _amount_of_rows; ++_row) {
      _stream << generate_row_insert(_table, _row, p_insert_definition)
              << std::endl;
    }

    return _stream.str();
  }

  std::string generate_inserts_fks(
    ptr<table> p_table,
    const insert_definition& p_insert_definition,
    foreigners_keys_created& p_foreigners_keys_created)
  {
    std::stringstream _stream;
    uint16_t _num_fks = p_table->get_num_fks();
    for (uint16_t _count_fk = 0; _count_fk < _num_fks; ++_count_fk) {
      ptr<foreign_key> _foreign_key = p_table->get_foregign_key(_count_fk);

      foreigners_keys_values::const_iterator _foreigners_keys_values_ite =
        p_insert_definition.get_foreigner_key_value(_foreign_key->get_name());

      _stream << generate_insert_fk(_foreign_key,
                                    *_foreigners_keys_values_ite,
                                    p_foreigners_keys_created)
              << std::endl;
    }
    return _stream.str();
  }

  std::string generate_insert_fk(
    ptr<foreign_key> /*p_fk*/,
    const foreign_key_value& /*p_fk_value*/,
    foreigners_keys_created& /*p_foreigners_keys_created*/)
  {
    std::stringstream _stream;
    return _stream.str();
  }

  std::string generate_row_insert(
    ptr<table> /*p_table*/,
    uint16_t /*p_row*/,
    const insert_definition& /*p_insert_definition*/)
  {
    std::stringstream _stream;

    return _stream.str();
  }

  //  std::string generate_table_insert_for_table(table* p_table, uint16_t
  //  p_num_lines)
  //  {
  //    std::stringstream _stream;

  //    uint16_t _num_fks = p_table->get_num_fks();
  //    for (uint16_t _count_fk = 0; _count_fk < _num_fks; ++_count_fk) {
  //      _stream << generate_table_insert_for_table(
  //                   p_table->get_foregign_key(_count_fk)->get_pk()->get_table(),
  //                   1)
  //              << std::endl;
  //    }

  //    return _stream.str();
  //  }

  ptr<table> find_table(const hosts& p_hosts,
                        const insert_definition& p_insert_definition) const
  {
    const host_name& _host_name = p_insert_definition.get_host_name();
    ptr<host> _host =
      p_hosts.find([&_host_name](const ptr<host>& p_host) -> bool {
        return p_host->get_name() == _host_name;
      });

    if (_host == nullptr) {
      return ptr<table>();
    }

    ptr<server> _server = _host->find(p_insert_definition.get_server_name());
    if (_server == nullptr) {
      return ptr<table>();
    }

    ptr<database> _database =
      _server->find(p_insert_definition.get_database_name());
    if (_database == nullptr) {
      return ptr<table>();
    }

    ptr<table> _table = _database->find(p_insert_definition.get_table_name());
    return _table;
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // generate_table_insert_H
