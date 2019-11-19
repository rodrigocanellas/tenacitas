#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLLECTION_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLLECTION_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>

#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

template<typename t_type>
struct collection
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const collection& p_collection)
  {
    p_out << "[";
    uint16_t _size = p_collection.get_size<uint16_t>();
    for (uint16_t _index = 0; _index < _size; ++_index) {
      p_out << *(p_collection.get(_index));
      if (_index != (_size - 1)) {
        p_out << ",";
      }
    }
    p_out << "]";

    return p_out;
  }

  void add(ptr<t_type> p_obj) { m_list.push_back(p_obj); }

  template<typename t_size>
  ptr<t_type> operator[](t_size p_index)
  {
    if (p_index >= static_cast<t_size>(m_list.size())) {
      return ptr<t_type>();
    }
    return *(std::next(m_list.begin(), p_index));
  }

  template<typename t_size>
  inline t_size get_size() const
  {
    return static_cast<t_size>(m_list.size());
  }

  ptr<t_type> find(
    std::function<bool(const ptr<t_type>& p_obj)> p_function) const
  {
    typename list::const_iterator _ite =
      std::find_if(m_list.begin(), m_list.end(), p_function);
    if (_ite != m_list.end()) {
      return *_ite;
    }
    return ptr<t_type>();
  }

private:
  typedef std::list<ptr<t_type>> list;

private:
  template<typename t_size>
  ptr<t_type> get(t_size p_index) const
  {
    if (p_index >= static_cast<t_size>(m_list.size())) {
      return ptr<t_type>();
    }
    return *(std::next(m_list.begin(), p_index));
  }

private:
  list m_list;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLLECTION_H
