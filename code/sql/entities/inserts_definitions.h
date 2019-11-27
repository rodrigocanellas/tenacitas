#ifndef CAPEMISA_SQL_ENTITIES_INSERTIONS_DEFINITIONS_H
#define CAPEMISA_SQL_ENTITIES_INSERTIONS_DEFINITIONS_H

#include <algorithm>
#include <list>

#include <sql/entities/insert_definition.h>

namespace capemisa {
namespace sql {
namespace entities {

struct inserts_definitions {
  typedef std::list<insert_definition>  insert_definition_list;
  typedef insert_definition_list::const_iterator const_iterator;

  const_iterator add(insert_definition && p_insertion_definition) {
    const_iterator _ite =
        std::find_if(m_list.begin(),
                     m_list.end(),
                     [&p_insertion_definition](const insert_definition & p_id)->bool {
      return (p_insertion_definition == p_id);
    });
    if (_ite != m_list.end()) {
      return _ite;
    }
    m_list.push_back(std::move(p_insertion_definition));
    return std::prev(m_list.end());
  }

  inline const_iterator begin()const{return m_list.begin();}
  inline const_iterator end()const{return m_list.end();}

private:
  insert_definition_list m_list;

};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // INSERTIONS_DEFINITIONS_H
