#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_H

#include <iostream>
#include <list>

#include <sql/entities/name.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table;

struct foreign_key
{

  friend std::ostream& operator<<(std::ostream& p_out, const foreign_key& p_fk);

  foreign_key() = delete;

  explicit foreign_key(const name& p_name, ptr<primary_key> p_primary_key)
    : m_name(p_name)
    , m_primary_key(p_primary_key)
  {}

  foreign_key(foreign_key&& p_foreign_key)
    : m_name(std::move(p_foreign_key.m_name))
    , m_primary_key(std::move(p_foreign_key.m_primary_key))
  {}

  inline const ptr<primary_key> get_pk() const { return m_primary_key; }

  inline const name& get_name() const { return m_name; }

private:
  name m_name;
  ptr<primary_key> m_primary_key;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_H
