#include <sql/entities/foreign_key.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const foreign_key& p_fk)
{
  p_out << "{ \"name\" : \"" << p_fk.get_name() << "\", "
        << "\"pk\" : { \"name\" : \"" << p_fk.get_pk()->get_table_name()
        << "\"}}";

  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa
