#ifndef TENACITAS_CONCURRENT_BUS_LOOP_H
#define TENACITAS_CONCURRENT_BUS_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <future>

#include <concurrent/bus/traits.h>
#include <logger/bus/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace bus {

///
/// \brief loop implements a generic loop
///
/// A loop needs a work function, that will execute a defined work at each round
/// of the loop; an amount of time that the work function has to execute; a
/// break function, that indicates when the loop should break; and a provide
/// function, that will provide data for the work function, if available
///
/// In this version of the loop class, the work function actually needs a data,
/// provided by the provide function, which type is defined by the type
/// parameter \p t_data.
///
/// \tparam t_data is the type of the data manipulated during the loop; it must
/// be:
///    - default constructible
///    - move constructible
///
template<typename t_data>
struct loop
{

    ///
    /// \brief work_t type of function to be called during the loop, responsible
    /// for doing the work defined
    ///
    /// loop_traits<t_data>::work_t evaluates to std::function<bool(t_data&&)>
    typedef typename loop_traits<t_data>::work_t work_t;

    ///
    /// \brief provide_t type of function that will provide, if available, an
    /// instance of \p t_data
    ///
    /// loop_traits<t_data>::provide_t evaluates to
    /// std::function<std::pair<bool, t_data>()>
    typedef typename loop_traits<t_data>::provide_t provide_t;

    ///
    /// \brief break_t type of function that will indicate that the loop must
    /// stop
    ///
    /// loop_traits<t_data>::break_t evaluates to std::function<bool()>
    typedef typename loop_traits<t_data>::break_t break_t;

    ///
    /// \brief loop constructor
    /// \param p_work instance of the function that will execute the defined
    /// work
    /// \param p_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    /// \param p_break instance of the function that will indicate when the loop
    /// must stop
    /// \param p_provide instance of the function that will provide an instance
    /// of \p t_data, if available
    ///
    loop(work_t&& p_work,
         std::chrono::milliseconds p_timeout,
         break_t&& p_break,
         provide_t&& p_provide)
      : m_work(std::move(p_work))
      , m_timeout(std::move(p_timeout))
      , m_break(std::move(p_break))
      , m_provide(std::move(p_provide))
      , m_stopped(true)
    {}

    /// \brief loop decault constuctor not allowed
    loop() = delete;

    /// \brief destructor
    ~loop() = default;

    /// \brief copy constructor not allowed
    loop(const loop&) = delete;

    ///
    /// \brief loop move constructor
    /// \param p_loop an instance o \p loop to be moved
    ///
    loop(loop&& p_loop) noexcept
      : m_work(std::move(p_loop.m_work))
      , m_timeout(std::move(p_loop.m_timeout))
      , m_break(std::move(p_loop.m_break))
      , m_provide(std::move(p_loop.m_provide))
      , m_stopped(true)
    {}

    /// \brief copy assignment not allowed
    loop& operator=(const loop&) = delete;

    /// \brief move assignment
    loop& operator=(loop&& p_loop) noexcept
    {
        if (this != &p_loop) {
            m_work = std::move(p_loop.m_work);
            m_timeout = std::move(p_loop.m_timeout);
            m_break = std::move(p_loop.m_break);
            m_provide = std::move(p_loop.m_provide);
            m_stopped = true;
        }
        return *this;
    }

    ///
    /// \brief is_stopped
    /// \return \p true if the loop is not running; \p false othewise
    ///
    inline bool is_stopped() const { return m_stopped; }

    ///
    /// \brief get_work
    /// \return a copy of the function that executes a defined work in each
    /// round of the loop
    ///
    inline work_t get_work() const { return m_work; }

    ///
    /// \brief get_break
    /// \return a copy of the function that can make the loop stop
    ///
    inline break_t get_break() const { return m_break; }

    ///
    /// \brief get_provide
    /// \return a copy of the function that provides an instance of \p t_data,
    /// if available, to the work function
    ///
    inline provide_t get_provide() const { return m_provide; }

    ///
    /// \brief get_timeout
    /// \return the amount of time that the loop will wait for the work function
    /// to finish
    ///
    inline std::chrono::milliseconds get_timeout() const { return m_timeout; }

    ///
    /// \brief stop forces the loop to stop
    ///
    inline void stop() { m_stopped = true; }

    ///
    /// \brief start initates the loop,
    ///
    /// If the loop was already initiated, it does not start a new loop
    ///
    void start()
    {
        if (!m_stopped) {
            cerr_debug(this, " not starting because it was not stopped");
            return;
        }

        m_stopped = false;

        while (true) {

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            cerr_debug(this, " waiting for data");
            std::pair<bool, t_data> _provide = m_provide();
            if (!_provide.first) {
                cerr_debug(this, " breaking because there is no data");
                break;
            }

            cerr_debug(this, " data received ", _provide.second);

            std::future<bool> _future = std::async(
              std::launch::async, std::ref(m_work), std::move(_provide.second));
            if (_future.wait_for(m_timeout) == std::future_status::ready) {
                if (!_future.get()) {
                    cerr_debug(this, " breaking because there is no more work");
                    m_stopped = true;
                    break;
                }
            } else {
                cerr_warn(this, " timeout for data ", _provide.second);
            }

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            if (m_break()) {
                cerr_debug(this, " stopping because breaker said so");
                m_stopped = true;
                break;
            }
        }
    }

  private:
    ///
    /// \brief m_work instance of the function that will execute the defined
    /// work
    ///
    work_t m_work;

    ///
    /// \brief m_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    ///
    std::chrono::milliseconds m_timeout;

    ///
    /// \brief m_break instance of the function that will indicate when the loop
    /// must stop
    ///
    break_t m_break;

    ///
    /// \brief m_provide instance of the function that will provide an instance
    /// of \p t_data, if available
    ///
    provide_t m_provide;

    ///
    /// \brief m_stopped indicates if the loop is running or not
    ///
    bool m_stopped;
};

///
/// \brief loop implements a generic loop
///
/// A loop needs a work function, that will execute a defined work at each round
/// of the loop; an amount of time that the work function has to execute; and a
/// break function, that indicates when the loop should break
///
/// In this specialization of the loop class, the work function actually does
/// needs a data, so the type parameter is set to \p void.
///
template<>
struct loop<void>
{
    ///
    /// \brief work_t type of function to be called during the loop, responsible
    /// for doing the work defined
    ///
    /// loop_traits<void>::work_t evaluates to std::function<bool(void)>
    typedef typename loop_traits<void>::work_t work_t;

    ///
    /// \brief provide_t type of function that will provide, if available, an
    /// instance of \p t_data
    ///
    /// loop_traits<t_data>::provide_t evaluates to
    /// std::function<void()>
    typedef typename loop_traits<void>::provide_t provide_t;

    ///
    /// \brief break_t type of function that will indicate that the loop must
    /// stop
    ///
    /// loop_traits<t_data>::break_t evaluates to std::function<bool()>
    typedef typename loop_traits<void>::break_t break_t;

    ///
    /// \brief loop constructor
    /// \param p_work instance of the function that will execute the defined
    /// work
    /// \param p_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    /// \param p_break instance of the function that will indicate when the loop
    /// must stop
    /// \param p_provide is a dummy paramter, to allow othe classes to use this
    /// specialization of \p loop, or the other one where the \p provide_t
    /// actually provides data
    ///
    loop(work_t&& p_work,
         std::chrono::milliseconds p_timeout,
         break_t&& p_break,
         provide_t&& p_provide)
      : m_work(std::move(p_work))
      , m_timeout(std::move(p_timeout))
      , m_break(std::move(p_break))
      , m_provide(std::move(p_provide))
      , m_stopped(true)
    {}

    /// \brief loop decault constuctor not allowed
    loop() = delete;

    /// \brief destructor
    ~loop() = default;

    /// \brief copy constructor not allowed
    loop(const loop&) = delete;

    /// \brief loop move constructor not allowed
    loop(loop&& p_loop) noexcept = delete;

    /// \brief copy assignment not allowed
    loop& operator=(const loop&) = delete;

    /// \brief move assignment not allowed
    loop& operator=(loop&& p_loop) noexcept = delete;

    ///
    /// \brief is_stopped
    /// \return \p true if the loop is not running; \p false othewise
    ///
    inline bool is_stopped() const { return m_stopped; }

    ///
    /// \brief get_work
    /// \return a copy of the function that executes a defined work in each
    /// round of the loop
    ///
    inline work_t get_work() const { return m_work; }

    ///
    /// \brief get_break
    /// \return a copy of the function that can make the loop stop
    ///
    inline break_t get_break() const { return m_break; }

    ///
    /// \brief get_provide
    /// \return a copy of the function that provides an instance of \p t_data,
    /// if available, to the work function
    ///
    inline provide_t get_provide() const { return m_provide; }

    ///
    /// \brief get_timeout
    /// \return the amount of time that the loop will wait for the work function
    /// to finish
    ///
    inline std::chrono::milliseconds get_timeout() const { return m_timeout; }

    ///
    /// \brief stop forces the loop to stop
    ///
    inline void stop() { m_stopped = true; }

    ///
    /// \brief start initates the loop,
    ///
    /// If the loop was already initiated, it does not start a new loop
    ///
    void start()
    {
        if (m_stopped == false) {
            cerr_debug(this, " not starting beacause it was not stopped");
            return;
        }

        m_stopped = false;

        while (true) {

            cerr_debug(this, " one more loop");

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            cerr_debug(this, " calling work");
            std::future<bool> _future =
              std::async(std::launch::async, std::ref(m_work));
            if (_future.wait_for(m_timeout) == std::future_status::ready) {
                if (!_future.get()) {
                    cerr_debug(this, " breaking because there is no more work");
                    m_stopped = true;
                    break;
                }
            } else {
                cerr_warn(this, " timeout");
            }

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            if (m_break()) {
                cerr_debug(this, " breaker said to stop");
                m_stopped = true;
                break;
            }
        }
    }

  private:
    ///
    /// \brief m_work instance of the function that will execute the defined
    /// work
    ///
    work_t m_work;

    ///
    /// \brief m_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    ///
    std::chrono::milliseconds m_timeout;

    ///
    /// \brief m_break instance of the function that will indicate when the loop
    /// must stop
    ///
    break_t m_break;

    ///
    /// \brief m_provide instance of the function that will provide an instance
    /// of \p t_data, if available
    ///
    provide_t m_provide;

    ///
    /// \brief m_stopped indicates if the loop is running or not
    ///
    bool m_stopped = true;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H
