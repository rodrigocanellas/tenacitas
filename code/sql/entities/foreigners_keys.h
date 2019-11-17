#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGNERS_KEYS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGNERS_KEYS_H

#include <iostream>
#include <list>

#include <sql/entities/collection.h>
#include <sql/entities/foreign_key.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef collection<foreign_key> foreigners_keys;

// struct foreigners_keys
//{

//  typedef collection<foreign_key> foreign_key_list;

//  typedef foreign_key_list::const_iterator const_iterator;
//  typedef foreign_key_list::iterator iterator;

//  friend std::ostream& operator<<(std::ostream& p_out,
//                                  const foreigners_keys& p_foreigners_keys);

//  inline iterator add(ptr<foreign_key> p_foreign_key)
//  {
//    //    return m_foreign_key_list.emplace(std::move(p_foreign_key)).first;
//    m_foreign_key_list.push_back(std::move(p_foreign_key));
//    return std::prev(m_foreign_key_list.end());
//  }

//  inline const_iterator end() const { return m_foreign_key_list.end(); }

//  inline const_iterator begin() const { return m_foreign_key_list.begin(); }

//  inline iterator end() { return m_foreign_key_list.end(); }

//  inline iterator begin() { return m_foreign_key_list.begin(); }

//  inline iterator find(const name& p_foreign_key_name)
//  {
//    return std::find_if(
//      m_foreign_key_list.begin(),
//      m_foreign_key_list.end(),
//      [&p_foreign_key_name](const ptr<foreign_key>& p_foreign_key) -> bool {
//        return p_foreign_key->get_name() == p_foreign_key_name;
//      });
//  }

// private:
//  foreign_key_list m_foreign_key_list;
//};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEYS_H
