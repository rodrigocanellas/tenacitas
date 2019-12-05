#ifndef CAPEMISA_SQL_ENTITIES_COLLECTION_H
#define CAPEMISA_SQL_ENTITIES_COLLECTION_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>

#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace generic {

/// \brief collection of pointer to a \p type
///
/// \tparam t_type must implement <tt>name get_name()</tt>
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

  template<typename... t_params>
  generic::ptr<t_type> add(const generic::name& p_name, t_params... p_params)
  {
    generic::ptr<t_type> _obj = find(p_name);
    if (_obj != nullptr) {
      return _obj;
    }

    _obj = generic::make_ptr<t_type>(p_name, p_params...);
    m_list.push_back(_obj);
    return _obj;
  }

  inline void add(generic::ptr<t_type> p_obj)
  {
    if (find(p_obj->get_name()) == nullptr) {
      m_list.push_back(p_obj);
    }
  }

  template<typename t_size>
  generic::ptr<t_type> operator[](t_size p_index) const
  {
    if (p_index >= static_cast<t_size>(m_list.size())) {
      return generic::ptr<t_type>();
    }
    return *(std::next(m_list.begin(), p_index));
  }

  template<typename t_size>
  inline t_size get_size() const
  {
    return static_cast<t_size>(m_list.size());
  }

  //  generic::ptr<t_type> find(
  //    std::function<bool(const generic::ptr<t_type>& p_obj)> p_function) const
  //  {
  //    typename list::const_iterator _ite =
  //      std::find_if(m_list.begin(), m_list.end(), p_function);
  //    if (_ite != m_list.end()) {
  //      return *_ite;
  //    }
  //    return generic::ptr<t_type>();
  //  }

  generic::ptr<t_type> find(const generic::name& p_name) const
  {
    typename list::const_iterator _ite =
      std::find_if(m_list.begin(),
                   m_list.end(),
                   [&p_name](const generic::ptr<t_type>& p_type) -> bool {
                     return p_type->get_name() == p_name;
                   });
    if (_ite != m_list.end()) {
      return *_ite;
    }
    return generic::ptr<t_type>();
  }

  void merge(collection& p_collection)
  {
    m_list.merge(p_collection.m_list,
                 [this, &p_collection](const generic::ptr<t_type> p_l,
                                       const generic::ptr<t_type> p_r) -> bool {
                   return p_l->get_name() == p_r->get_name();
                 });
  }

private:
  typedef std::list<generic::ptr<t_type>> list;

private:
  template<typename t_size>
  generic::ptr<t_type> get(t_size p_index) const
  {
    if (p_index >= static_cast<t_size>(m_list.size())) {
      return generic::ptr<t_type>();
    }
    return *(std::next(m_list.begin(), p_index));
  }

private:
  list m_list;
};

} // namespace generic
} // namespace sql
} // namespace capemisa

#endif // COLLECTION_H
