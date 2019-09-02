#ifndef TENACITAS_CONCURRENT_BUS_LOOP_H
#define TENACITAS_CONCURRENT_BUS_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <future>

#include <concurrent/business/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace business {

///
/// \brief loop implements a generic loop
///
/// A loop needs a work function, that will execute a defined work at each round
/// of the loop; an amount of time that the work function has to execute; a
/// break function, that indicates when the loop should break; and a provider
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
/// \tparam t_log provides log funcionality:
/// static void set_debug()
/// static void set_info()
/// static void set_warn()
/// static void set_error()
/// static void debug(const std::string & p_class, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_class, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_class, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_class, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_class, int p_line, const
/// t_params&... p_params)
template<typename t_data, typename t_log>
struct loop_t
{

    ///
    /// \brief worker type of function to be called during the loop, responsible
    /// for doing the work defined
    ///
    /// loop_traits<t_data>::work_t evaluates to std::function<bool(t_data&&)>
    typedef typename loop_traits_t<t_data>::worker worker;

    ///
    /// \brief provider type of function that will provide, if available, an
    /// instance of \p t_data
    ///
    /// loop_traits<t_data>::provide_t evaluates to
    /// std::function<std::pair<bool, t_data>()>
    typedef typename loop_traits_t<t_data>::provider provider;

    ///
    /// \brief breaker type of function that will indicate that the loop must
    /// stop
    ///
    /// loop_traits<t_data>::break_t evaluates to std::function<bool()>
    typedef typename loop_traits_t<t_data>::breaker breaker;

    ///
    /// \brief log alias for @p t_log
    ///
    typedef t_log log;

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
    loop_t(worker&& p_work,
           std::chrono::milliseconds p_timeout,
           breaker&& p_break,
           provider&& p_provide)
      : m_work(std::move(p_work))
      , m_timeout(std::move(p_timeout))
      , m_break(std::move(p_break))
      , m_provide(std::move(p_provide))
      , m_stopped(true)
    {}

    /// \brief loop decault constuctor not allowed
    loop_t() = delete;

    /// \brief destructor
    ~loop_t() = default;

    /// \brief copy constructor not allowed
    loop_t(const loop_t&) = delete;

    ///
    /// \brief loop move constructor
    /// \param p_loop an instance o \p loop to be moved
    ///
    loop_t(loop_t&& p_loop) noexcept
      : m_work(std::move(p_loop.m_work))
      , m_timeout(std::move(p_loop.m_timeout))
      , m_break(std::move(p_loop.m_break))
      , m_provide(std::move(p_loop.m_provide))
      , m_stopped(true)
    {}

    /// \brief copy assignment not allowed
    loop_t& operator=(const loop_t&) = delete;

    /// \brief move assignment
    loop_t& operator=(loop_t&& p_loop) noexcept
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
    inline worker get_worker() const { return m_work; }

    ///
    /// \brief get_break
    /// \return a copy of the function that can make the loop stop
    ///
    inline breaker get_breaker() const { return m_break; }

    ///
    /// \brief get_provide
    /// \return a copy of the function that provides an instance of \p t_data,
    /// if available, to the work function
    ///
    inline provider get_provider() const { return m_provide; }

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
            log::debug(m_name,
                       __LINE__,
                       this,
                       " not starting because it was not stopped");
            return;
        }

        m_stopped = false;

        while (true) {

            if (m_stopped) {
                log::debug(m_name, __LINE__, this, " stopping loop");
                break;
            }

            log::debug(m_name, __LINE__, this, " waiting for data");
            std::pair<bool, t_data> _provide = m_provide();
            if (!_provide.first) {
                log::debug(
                  m_name, __LINE__, this, " breaking because there is no data");
                break;
            }

            log::debug(
              m_name, __LINE__, this, " data received ", _provide.second);

            std::future<bool> _future = std::async(
              std::launch::async, std::ref(m_work), std::move(_provide.second));
            if (_future.wait_for(m_timeout) == std::future_status::ready) {
                if (!_future.get()) {
                    log::debug(m_name,
                               __LINE__,
                               this,
                               " breaking because there is no more work");
                    m_stopped = true;
                    break;
                }
            } else {
                log::warn(this, " timeout for data ", _provide.second);
            }

            if (m_stopped) {
                log::debug(m_name, __LINE__, this, " stopping loop");
                break;
            }

            if (m_break()) {
                log::debug(
                  m_name, __LINE__, this, " stopping because breaker said so");
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
    worker m_work;

    ///
    /// \brief m_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    ///
    std::chrono::milliseconds m_timeout;

    ///
    /// \brief m_break instance of the function that will indicate when the loop
    /// must stop
    ///
    breaker m_break;

    ///
    /// \brief m_provide instance of the function that will provide an instance
    /// of \p t_data, if available
    ///
    provider m_provide;

    ///
    /// \brief m_stopped indicates if the loop is running or not
    ///
    bool m_stopped;

    static const std::string m_name;
};

template<typename t_data, typename t_log>
const std::string loop_t<t_data, t_log>::m_name("loop");

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
/// \tparam t_log provides log funcionality:
/// static void set_debug()
/// static void set_info()
/// static void set_warn()
/// static void set_error()
/// static void debug(const std::string & p_class, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_class, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_class, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_class, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_class, int p_line, const
/// t_params&... p_params)
template<typename t_log>
struct loop_t<void, t_log>
{
    ///
    /// \brief work_t type of function to be called during the loop, responsible
    /// for doing the work defined
    ///
    /// loop_traits<void>::work_t evaluates to std::function<bool(void)>
    typedef typename loop_traits_t<void>::worker worker;

    ///
    /// \brief provide_t type of function that will provide, if available, an
    /// instance of \p t_data
    ///
    /// loop_traits<t_data>::provide_t evaluates to
    /// std::function<void()>
    typedef typename loop_traits_t<void>::provider provider;

    ///
    /// \brief break_t type of function that will indicate that the loop must
    /// stop
    ///
    /// loop_traits<t_data>::break_t evaluates to std::function<bool()>
    typedef typename loop_traits_t<void>::breaker breaker;

    ///
    /// \brief log alias for @p t_log
    ///
    typedef t_log log;

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
    loop_t(worker&& p_work,
           std::chrono::milliseconds p_timeout,
           breaker&& p_break,
           provider&& p_provide)
      : m_work(std::move(p_work))
      , m_timeout(std::move(p_timeout))
      , m_break(std::move(p_break))
      , m_provide(std::move(p_provide))
      , m_stopped(true)
    {}

    /// \brief loop decault constuctor not allowed
    loop_t() = delete;

    /// \brief destructor
    ~loop_t() = default;

    /// \brief copy constructor not allowed
    loop_t(const loop_t&) = delete;

    /// \brief loop move constructor not allowed
    loop_t(loop_t&& p_loop) noexcept = delete;

    /// \brief copy assignment not allowed
    loop_t& operator=(const loop_t&) = delete;

    /// \brief move assignment not allowed
    loop_t& operator=(loop_t&& p_loop) noexcept = delete;

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
    inline worker get_worker() const { return m_work; }

    ///
    /// \brief get_break
    /// \return a copy of the function that can make the loop stop
    ///
    inline breaker get_breaker() const { return m_break; }

    ///
    /// \brief get_provide
    /// \return a copy of the function that provides an instance of \p t_data,
    /// if available, to the work function
    ///
    inline provider get_provider() const { return m_provide; }

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
            log::debug(m_name,
                       __LINE__,
                       this,
                       " not starting beacause it was not stopped");
            return;
        }

        m_stopped = false;

        while (true) {

            log::debug(m_name, __LINE__, this, " one more loop");

            if (m_stopped) {
                log::debug(m_name, __LINE__, this, " stopping loop");
                break;
            }

            log::debug(m_name, __LINE__, this, " calling work");
            std::future<bool> _future =
              std::async(std::launch::async, std::ref(m_work));
            if (_future.wait_for(m_timeout) == std::future_status::ready) {
                if (!_future.get()) {
                    log::debug(m_name,
                               __LINE__,
                               this,
                               " breaking because there is no more work");
                    m_stopped = true;
                    break;
                }
            } else {
                log::warn(this, " timeout");
            }

            if (m_stopped) {
                log::debug(m_name, __LINE__, this, " stopping loop");
                break;
            }

            if (m_break()) {
                log::debug(m_name, __LINE__, this, " breaker said to stop");
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
    worker m_work;

    ///
    /// \brief m_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    ///
    std::chrono::milliseconds m_timeout;

    ///
    /// \brief m_break instance of the function that will indicate when the loop
    /// must stop
    ///
    breaker m_break;

    ///
    /// \brief m_provide instance of the function that will provide an instance
    /// of \p t_data, if available
    ///
    provider m_provide;

    ///
    /// \brief m_stopped indicates if the loop is running or not
    ///
    bool m_stopped = true;

    static const std::string m_name;
};
template<typename t_log>
const std::string loop_t<void, t_log>::m_name("loop");

} // namespace business
} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H
