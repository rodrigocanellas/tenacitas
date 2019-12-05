#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_VALUES_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_VALUES_H

#include <list>
#include <string>
#include <utility>

#include <sql/entities/column.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column_values
{

  typedef std::list<value> list;
  typedef list::const_iterator const_iterator;

  column_values() = delete;

  inline explicit column_values(generic::ptr<column> p_column)
    : m_column(p_column)
  {}

  inline const_iterator add(value&& p_value)
  {
    m_list.push_back(std::move(p_value));
    return std::prev(m_list.end());
  }

  inline const_iterator begin() const { return m_list.begin(); }

  inline const_iterator end() const { return m_list.end(); }

  inline generic::ptr<column> get_column() const { return m_column; }

  inline uint16_t get_size() const { return m_list.size(); }

  const value& get_value(uint16_t p_counter) const
  {
    return *(std::next(m_list.begin(), p_counter));
  }

private:
  generic::ptr<column> m_column;
  list m_list;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // VALUES_H
