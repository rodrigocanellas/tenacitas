/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_INTERNAL_HANDLER_ID_H
#define TENACITAS_LIB_ASYNC_INTERNAL_HANDLER_ID_H

#include <typeinfo>

#include <tenacitas.lib/traits/event.h>
#include <tenacitas.lib/traits/handler.h>

namespace tenacitas::lib::async::internal {

using handler_id = size_t;

template <traits::event t_event, traits::handler<t_event> t_handler>
inline handler_id get_handler_id() {
  return static_cast<handler_id>(typeid(t_handler).hash_code());
};

// template <traits::event t_event>

// std ::array<uint8_t, sizeof(void *)>
// handler_id(const async::handler<t_event> &p_handler) {
//   uint8_t *_b = (uint8_t *)&p_handler;
//   uint8_t *_e = _b + sizeof(void *);

//   std ::array<uint8_t, sizeof(void *)> _id;
//   uint8_t _count{0};
//   for (uint8_t *_i = _b; _i != _e; ++_i) {
//     _id[_count++] = *_i;
//   }
//   return _id;
// };

// struct handler_id final {
//   handler_id() = delete;

//   template <traits::event t_event>
//   handler_id(const async::handler<t_event> *const p_handler) {
//     uint8_t *_b = (uint8_t *)&p_handler;
//     uint8_t *_e = _b + sizeof(void *);

//     uint8_t _count{0};
//     for (uint8_t *_i = _b; _i != _e; ++_i) {
//       m_id[_count++] = *_i;
//     }
//   }

//   handler_id(const handler_id &) = default;
//   handler_id(handler_id &&) = default;
//   ~handler_id() = default;

//   handler_id &operator=(const handler_id &) = default;
//   handler_id &operator=(handler_id &&) = default;

//   constexpr bool operator==(const handler_id &p_handler_id) const {
//     uint8_t _count{0};
//     for (auto _i : m_id) {
//       if (_i != p_handler_id.m_id[_count++]) {
//         return false;
//       }
//     }
//     return true;
//   }

//   friend std::ostream &operator<<(std::ostream &out, handler_id
//   &p_handler_id) {
//     for (auto _i : p_handler_id.m_id) {
//       out << static_cast<uint16_t>(_i);
//     }
//     return out;
//   }

// private:
//   using id = std::array<uint8_t, sizeof(void *)>;

// private:
//   id m_id;
// };

} // namespace tenacitas::lib::async::internal

#endif
