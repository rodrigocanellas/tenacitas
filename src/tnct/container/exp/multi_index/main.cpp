/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <iostream>

#include "tnct/container/dat/multi_index.h"
#include "tnct/tuple/cpt/is_tuple.h"

template <tnct::tuple::cpt::is_tuple t_object, std::size_t t_key_pos>
struct map_index_definition {
  static constexpr std::size_t key_pos = t_key_pos;
  using index_id = tnct::container::cpt::std_map_id;
};

template <tnct::tuple::cpt::is_tuple t_object, std::size_t t_key_pos>
struct multimap_index_definition {
  static constexpr std::size_t key_pos = t_key_pos;
  using index_id = tnct::container::cpt::std_multimap_id;
};

using xpto = std::tuple<std::int16_t, float, std::string>;

using xpto_indexes =
    tnct::container::dat::multi_index_t<xpto,
                                        multimap_index_definition<xpto, 0>,
                                        multimap_index_definition<xpto, 1>>;

using xpto_const_ref = typename xpto_indexes::object_const_ref;

using xpto_ref = typename xpto_indexes::object_ref;

int main() {

  xpto_indexes _xpto_indexes;

  std::cout << "\n###### Adding\n";
  _xpto_indexes.add(xpto{-9, 3.14, "hi"});
  _xpto_indexes.add(xpto{138, 0.72, "hello"});
  _xpto_indexes.add(xpto{138, -4.21, "bye"});
  _xpto_indexes.add(xpto{-2, -7.85, "see u"});

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Searching in index 0 key 138\n";
  std::vector<xpto_ref> _objects{_xpto_indexes.get<0>(138)};

  std::for_each(_objects.begin(), _objects.end(),
                [](const xpto_const_ref &p_obj) {
                  std::cout << "object = " << p_obj.get().value() << '\n';
                });

  std::cout << "\n###### Searching in index 1 key -4.21\n";
  _objects = _xpto_indexes.get<1>(-4.21);

  std::for_each(_objects.begin(), _objects.end(),
                [](const xpto_const_ref &p_obj) {
                  std::cout << "object = " << p_obj.get().value() << '\n';
                });

  std::cout << "\n###### Deleting from index 1 key 3.14\n";
  _xpto_indexes.erase<1>(float{3.14});

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Find from index 0 where key is -2 and update field 1 "
               "from -7.85 to 0.38\n";
  _objects = _xpto_indexes.get<0>(-2);
  if (_objects.size() != 1) {
    std::cout << "ERROR: found " << _objects.size() << std::endl;
    return -1;
  }

  xpto_ref _ref{_objects[0]};
  _xpto_indexes.update<1>(_ref, 0.38);
  std::cout << _xpto_indexes << std::endl;
}
