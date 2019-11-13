#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_H

#include <list>

#include <sql/entities/primary_key.h>


namespace capemisa {
namespace sql {
namespace entities {


struct foreign_key {

  typedef const primary_key const* primary_key_ptr;

  foreign_key()=delete;

  explicit foreign_key(primary_key_ptr p_primary_key)
    : m_primary_key(p_primary_key) {}

  const primary_key & get_pk() const {
    return *m_primary_key;
  }


private:
  primary_key_ptr m_primary_key;
};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // FOREIGN_KEY_H
