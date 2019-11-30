
#include <cstdint>
#include <iostream>

#include <sql/entities/sequential_value_generator.h>
#include <sql/entities/value.h>
#include <sql/entities/values.h>

namespace sql_ent = capemisa::sql::entities;

void
sequential()
{
  sql_ent::sequential_value_generator<uint32_t> _svg(14, 5);

  sql_ent::values _values = _svg.generate(8);

  uint16_t _counter = 0;
  for (const sql_ent::value& _value : _values) {
    std::cout << "value #" << ++_counter << " = " << _value.get_value()
              << std::endl;
  }
}

int
main()
{
  sequential();
}
