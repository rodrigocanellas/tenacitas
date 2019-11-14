//#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_H
//#define CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_H

//#include <iostream>
//#include <list>

//#include <sql/entities/primary_key.h>
//#include <sql/entities/name.h>

//namespace capemisa {
//namespace sql {
//namespace entities {

//template <typename t_tables>
//struct foreign_key_t {

//  typedef t_tables table;
//  typedef typename table::primary_key_ptr primary_key_ptr;

//  friend std::ostream & operator<<(std::ostream & p_out, const foreign_key_t & p_fk) {
//    p_out << "{ \"name\" : \"" << p_fk.get_name() << "\", "
//          << "\"pk\" : { \"name\" : \""  << p_fk.get_pk().get_table().get_name() << "\"}}";

//    return p_out;
//  }

//  foreign_key_t()=delete;

//  explicit foreign_key_t(const name & p_name, primary_key_ptr p_primary_key)
//    : m_name(p_name),
//      m_primary_key(p_primary_key) {}

//  inline const primary_key_ptr get_pk() const {
//    return m_primary_key;
//  }

//  inline const name & get_name()const{return m_name;}


//private:
//  name m_name;
//  primary_key_ptr m_primary_key;
//};

//} // namespace entities
//} // namespace sql
//} // namespace capemisa


//#endif // FOREIGN_KEY_H
