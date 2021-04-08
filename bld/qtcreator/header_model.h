#ifndef TENACITAS_CONCURRENT_TIME_OUT_H
#define TENACITAS_CONCURRENT_TIME_OUT_H

/** @file */


#include <cstddef>
#include <exception>
#include <new>
#include <memory>
#include <cstdint>

/** @brief namespace of the organization */
namespace tenacitas {
/** @brief namespace of the project */
namespace concurrent {

/**
 * @brief The time_out class
 */
class time_out {

public:


    /** @brief @brief not allowed */
    inline time_out() = delete;

    /** @brief not allowed */
    inline time_out(const time_out &) = delete;

    /** @brief not allowed */
    inline time_out(time_out &&) = delete;

    /** Destructor */
    inline ~time_out();

    /** @brief not allowed */
    inline time_out & operator=(const time_out &) = delete;

    /** @brief not allowed */
    inline time_out & operator=(time_out &&) = delete;

    /** @brief not allowed */
    void *operator new[] (size_t) = delete;

    /** @brief not allowed */
    void operator delete[] (void *) = delete;

    /** @brief not allowed */
    void operator delete (void * p) = delete;

    /** @brief not allowed */
    void *operator new(size_t) = delete;


    private:
};
} // namespace concurrent
} // namespace tenacitas


#endif
