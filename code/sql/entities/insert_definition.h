#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_INSERT_DEFINITION_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_INSERT_DEFINITION_H

#include <cstdint>
#include <list>
#include <utility>

#include <sql/entities/column_value.h>
#include <sql/entities/column_value_pattern.h>
#include <sql/entities/columns_values.h>
#include <sql/entities/foreign_key_value.h>
#include <sql/entities/foreign_key_value_pattern.h>
#include <sql/entities/foreigners_keys_values.h>
#include <sql/entities/names.h>

namespace capemisa {
namespace sql {
namespace entities {

struct insert_definition
{

  insert_definition() = delete;

  insert_definition(host_name p_host_name,
                    server_name p_server_name,
                    database_name p_database_name,
                    table_name p_table_name,
                    uint16_t p_amount_of_rows = 1)
    : m_host_name(p_host_name)
    , m_server_name(p_server_name)
    , m_database_name(p_database_name)
    , m_table_name(p_table_name)
    , m_amount_of_rows(p_amount_of_rows)
  {}

  inline columns_values::const_iterator add_column_value(
    const column_name& p_column_name,
    column_value_pattern p_column_value_pattern)
  {
    return m_columns_values.add(p_column_name, p_column_value_pattern);
  }

  inline foreigners_keys_values::const_iterator add_foreign_key_value(
    const foreign_key_name& p_foreign_key_name,
    foreign_key_value_pattern p_foreign_key_value_pattern,
    column_value_pattern p_column_value_pattern)
  {
    return m_foreigners_keys_values.add(
      p_foreign_key_name, p_foreign_key_value_pattern, p_column_value_pattern);
  }

  inline const host_name& get_host_name() const { return m_host_name; }
  inline const server_name& get_server_name() const { return m_server_name; }
  inline const database_name& get_database_name() const
  {
    return m_database_name;
  }
  inline const table_name& get_table_name() const { return m_table_name; }
  inline uint16_t get_amount_of_rows() const { return m_amount_of_rows; }

  inline columns_values::const_iterator begin_column_value() const
  {
    return m_columns_values.begin();
  }

  inline columns_values::const_iterator end_column_value() const
  {
    return m_columns_values.end();
  }

  inline const foreigners_keys_values::const_iterator begin_foreign_key_value()
    const
  {
    return m_foreigners_keys_values.begin();
  }

  inline const foreigners_keys_values::const_iterator end_foreign_key_value()
    const
  {
    return m_foreigners_keys_values.end();
  }

  bool operator==(const insert_definition& p_insert_definition) const
  {
    return ((m_host_name == p_insert_definition.m_host_name) &&
            (m_server_name == p_insert_definition.m_server_name) &&
            (m_database_name == p_insert_definition.m_database_name) &&
            (m_table_name == p_insert_definition.m_table_name));
  }

  foreigners_keys_values::const_iterator get_foreigner_key_value(
    const foreign_key_name& p_fk_name) const
  {
    return m_foreigners_keys_values.find(p_fk_name);
  }

private:
  host_name m_host_name;
  server_name m_server_name;
  database_name m_database_name;
  table_name m_table_name;
  uint16_t m_amount_of_rows;
  columns_values m_columns_values;
  foreigners_keys_values m_foreigners_keys_values;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // INSERTION_CONFIGURATION_H
