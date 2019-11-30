#ifndef CAPEMISA_SQL_FOREIGN_KEY_COLUMN_H
#define CAPEMISA_SQL_FOREIGN_KEY_COLUMN_H

#include <iostream>

#include <sql/entities/internal/collection.h>
#include <sql/entities/internal/types.h>
#include <sql/entities/name.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/size.h>

namespace capemisa {
namespace sql {
namespace entities {

struct foreign_key;

struct foreign_key_column : public column
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const foreign_key_column& p_fk);

  foreign_key_column() = delete;
  foreign_key_column(const foreign_key_column&) = delete;
  foreign_key_column(foreign_key_column&&) = delete;
  foreign_key_column& operator=(const foreign_key_column&) = delete;
  foreign_key_column& operator=(foreign_key_column&&) = delete;
  ~foreign_key_column() = default;

  foreign_key_column(const name& p_name,
                     const foreign_key* p_foreign_key,
                     ptr<primary_key_column> p_primary_key_column)
    : column(p_name,
             p_primary_key_column->get_type(),
             p_primary_key_column->get_size(),
             column::usage::foreign_key)
    , m_foreign_key(p_foreign_key)
    , m_primary_key_column(p_primary_key_column)
  {}

  const foreign_key& get_foreign_key() const;

  inline ptr<primary_key_column> get_pk_column() const
  {
    return m_primary_key_column;
  }

private:
  const foreign_key* m_foreign_key;
  ptr<primary_key_column> m_primary_key_column;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_COLUMN_H
