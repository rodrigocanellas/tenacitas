#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H

#include <list>

#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef std::list<table> tables;
typedef tables::const_iterator table_ptr;

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // TABLES_H
