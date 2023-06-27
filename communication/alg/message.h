#ifndef TENACITAS_LIB_ASYNC_ALG_MESSAGE_H
#define TENACITAS_LIB_ASYNC_ALG_MESSAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iomanip>
#include <sstream>
#include <string>

#include <tenacitas.lib/communication/cpt/concepts.h>

namespace tenacitas::lib::communication::alg {

std::string msg2str(const cpt::message auto &p_message) {
  std::stringstream _stream;
  _stream << '{';
  auto _ite = p_message.begin();
  auto _end{p_message.end()};
  if (_ite == _end) {
    _stream << "{}";
  } else {
    --_end;
    for (; _ite != _end; ++_ite) {
      _stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
              << std::right << static_cast<uint16_t>(*_ite) << ' ';
    }
    _stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << std::right << static_cast<uint16_t>(*_ite);
    _stream << '}';
  }
  return _stream.str();
}
/*
main.cpp:72:15: No matching function for call to 'msg2str'

message.h:17:13: candidate template ignored: constraints not satisfied [with
p_message:auto = message]

message.h:17:27: because 'std::vector<std::byte>' does not satisfy 'message'

concepts.h:28:22: because type constraint
'std::same_as<__gnu_cxx::__normal_iterator<std::byte *, std::vector<std::byte>
>, typename vector<byte>::const_iterator>' was not satisfied:

/usr/include/c++/11/concepts:63:9: note: because
'__detail::__same_as<__gnu_cxx::__normal_iterator<std::byte *,
std::vector<std::byte> >, __gnu_cxx::__normal_iterator<const std::byte *,
std::vector<std::byte> > >' evaluated to false

/usr/include/c++/11/concepts:57:27: note: because
'std::is_same_v<__gnu_cxx::__normal_iterator<std::byte *, std::vector<std::byte>
>, __gnu_cxx::__normal_iterator<const std::byte *, std::vector<std::byte> > >'
evaluated to false
*/
} // namespace tenacitas::lib::communication::alg

#endif
