/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <optional>

#include "tnct/container/dat/chuncked_container.h"

#include <iostream>

int main() {
  constexpr std::size_t _size{5};

  using chuncked_container =
      tnct::container::dat::chunked_container<int, _size>;

  chuncked_container _chuncked_container{-3};

  chuncked_container ::iterator _chuncked_container_ite{
      _chuncked_container.begin()};

  std::cout << (*_chuncked_container_ite).value() << std::endl;
  std::cout << _chuncked_container_ite->value() << std::endl;

  std::cout << _chuncked_container[0].value().get().value() << std::endl;

  typename chuncked_container::ref _reference{_chuncked_container.add(91)};
  std::cout << _reference.get().value() << std::endl;

  typename chuncked_container::const_ref _const_reference{
      _chuncked_container.add(-104)};
  std::cout << _const_reference.get().value() << std::endl;

  _chuncked_container.remove(2);

  if (_chuncked_container[2].value().get().has_value()) {
    std::cout << "ERROR, index 2 should be nullopt";
  } else {
    std::cout << "index 2 is nullopt, as expected";
  }
  std::cout << std::endl;

  _chuncked_container.remove(_chuncked_container.begin());

  if (_chuncked_container.begin()->has_value()) {
    std::cout << "ERROR, begin() should be nullopt";
  } else {
    std::cout << "begin() is nullopt, as expected";
  }
  std::cout << std::endl;

  typename chuncked_container::iterator _const_ite{
      ++_chuncked_container.begin()};

  _chuncked_container.remove(_const_ite);

  if (std::next(_chuncked_container.begin())->has_value()) {
    std::cout << "ERROR, ++begin() should be nullopt";
  } else {
    std::cout << "++begin() is nullopt, as expected";
  }
  std::cout << std::endl;

  typename chuncked_container::iterator _ite{_chuncked_container.begin()};
  *_ite = 102;
  ++_ite;
  *_ite = -201;

  _ite = _chuncked_container.begin();
  std::cout << _ite->value() << std::endl;
  ++_ite;
  std::cout << _ite->value() << std::endl;

  return 0;
}
