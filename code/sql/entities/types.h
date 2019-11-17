#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TYPES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TYPES_H

#include <memory>

namespace capemisa {
namespace sql {
namespace entities {

template<class t_class>
using ptr = std::shared_ptr<t_class>;

template<typename t_class, typename... t_params>
static ptr<t_class>
make_ptr(t_params... p_params)
{
  return std::make_shared<t_class>(p_params...);
}

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TYPES_H
