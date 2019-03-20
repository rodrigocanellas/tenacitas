#ifndef TENACITAS_CONCURRENT_BUS_TRAITS_H
#define TENACITAS_CONCURRENT_BUS_TRAITS_H

/// @author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <utility>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
namespace bus {

template<typename t_data>
struct loop_traits
{
    typedef std::function<bool(t_data&&)> work_t;
    typedef std::function<std::pair<bool, t_data>()> provide_t;
    typedef std::function<bool()> break_t;
};

template<>
struct loop_traits<void>
{
    typedef std::function<bool()> work_t;
    typedef std::function<void()> provide_t;
    typedef std::function<bool()> break_t;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TRAITS_H
