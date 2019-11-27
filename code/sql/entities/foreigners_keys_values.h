#ifndef CAPEMISA_SQL_ENTITIES_FOREIGNERS_KEYS_VALUES_H
#define CAPEMISA_SQL_ENTITIES_FOREIGNERS_KEYS_VALUES_H

#include <algorithm>
#include <list>

#include <sql/entities/column_value.h>
#include <sql/entities/foreign_key_value.h>
#include <sql/entities/names.h>

namespace capemisa {
namespace sql {
namespace entities {

struct foreigners_keys_values
{
  typedef std::list<foreign_key_value> foreign_key_value_list;
  typedef foreign_key_value_list::const_iterator const_iterator;

  const_iterator add(const foreign_key_name& p_foreign_key_name,
                     foreign_key_value_pattern p_foreign_key_value_pattern,
                     column_value_pattern p_column_value_pattern)
  {

    const_iterator _ite = std::find_if(
      m_list.begin(),
      m_list.end(),
      [&p_foreign_key_name](const foreign_key_value& p_fkv) -> bool {
        return p_fkv.get_foreign_key_name() == p_foreign_key_name;
      });
    if (_ite != m_list.end()) {
      return _ite;
    }
    m_list.push_back(foreign_key_value(
      p_foreign_key_name, p_foreign_key_value_pattern, p_column_value_pattern));
    return std::prev(m_list.end());
  }

  const_iterator begin() const { return m_list.begin(); }

  const_iterator end() const { return m_list.end(); }

  const_iterator find(const foreign_key_name& p_fk_name) const
  {
    return std::find_if(m_list.begin(),
                        m_list.end(),
                        [&p_fk_name](const foreign_key_value& p_fkv) -> bool {
                          return p_fkv.get_foreign_key_name() == p_fk_name;
                        });
  }

private:
  foreign_key_value_list m_list;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // FOREIGNERS_KEYS_VALUES_H
