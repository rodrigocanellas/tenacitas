#ifndef CAPEMISA_SQL_GENERIC_PTR_H
#define CAPEMISA_SQL_GENERIC_PTR_H

#include <memory>

namespace capemisa {
namespace sql {
namespace generic {

template<class t_class>
using ptr = std::shared_ptr<t_class>;

template<typename t_class, typename... t_params>
static ptr<t_class>
make_ptr(t_params... p_params)
{
  return std::make_shared<t_class>(p_params...);
}

} // namespace generic
} // namespace sql
} // namespace capemisa

#endif // PTR_H
