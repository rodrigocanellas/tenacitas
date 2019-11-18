#ifndef CAPEMISA_TEST_AUTOMATION_BUSINESS_INSERTION_CONFIGURATION_H
#define CAPEMISA_TEST_AUTOMATION_BUSINESS_INSERTION_CONFIGURATION_H

#include <cstdint>
#include <list>
#include <utility>

#include <sql/entities/name.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;

typedef name host_name;
typedef name server_name;
typedef name database_name;
typedef name table_name;

struct table_definition
{

  table_definition() = delete;

  table_definition(host_name p_host_name,
                   server_name p_server_name,
                   database_name p_database_name,
                   table_name p_table_name)
    : m_host_name(p_host_name)
    , m_server_name(p_server_name)
    , m_database_name(p_database_name)
    , m_table_name(p_table_name)
  {}

  host_name m_host_name;
  server_name m_server_name;
  database_name m_database_name;
  table_name m_table_name;
};

///
/// \brief num_lines_in_table number of lines to be inserted in a table
///
typedef std::pair<table_definition, uint16_t> num_lines_in_table;

typedef std::list<num_lines_in_table> num_lines_in_tables;

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // INSERTION_CONFIGURATION_H
