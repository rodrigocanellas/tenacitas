#ifndef TENACITAS_ASYNC_H
#define TENACITAS_ASYNC_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <set>
#include <thread>
#include <vector>

#include <tenacitas/calendar.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/type.h>

using namespace std::chrono_literals;

/// TODO improve documentation

/// \brief master namespace
namespace tenacitas {

/// \brief support for async (asynchronous) programming
namespace async {

using namespace tenacitas::type;

/// \brief
typedef uint8_t priority;

namespace internal {
/// \brief Type of time used to define timeout
typedef std::chrono::milliseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::milliseconds interval;

template <typename t_type>
struct executer_traits_t {
    /// \brief returned by the executed function
    typedef t_type type;

    /// \brief result of the time controlled execution of the
    /// function
    typedef std::optional<type> result;
};

template <>
struct executer_traits_t<void> {
    /// \brief returned by the executed function
    typedef void type;

    /// \brief result of the time controlled execution of the
    /// function
    typedef bool result;
};

template <typename t_ret>
struct executer_t {
    template <typename t_time, typename t_function, typename... t_params>
    std::optional<t_ret> operator()(t_time p_timeout,
                                    t_function p_function,
                                    std::tuple<t_params...> &&p_tuple) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        std::tuple<ptr<bool>, t_params...> _tuple =
            std::tuple_cat(std::make_tuple(_stop), std::move(p_tuple));

        t_ret _ret;

        std::thread _th([&p_function, &_cond, &_ret, &_tuple]() -> void {
            _ret = std::apply(p_function, std::move(_tuple));
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return {_ret};
        }

        *_stop = true;
        _th.join();
        return {};
    }

    template <typename t_time, typename t_function, typename t_param>
    std::optional<t_ret>
    operator()(t_time p_timeout, t_function p_function, t_param &&p_param) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        t_ret _ret;

        std::thread _th(
            [&p_function, _stop, &_cond, &_ret, &p_param]() -> void {
                _ret = p_function(_stop, std::move(p_param));
                _cond.notify_one();
            });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return {_ret};
        }

        *_stop = true;
        _th.join();
        return {};
    }

    template <typename t_time, typename t_function>
    std::optional<t_ret> operator()(t_time p_timeout, t_function p_function) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        t_ret _ret;

        std::thread _th([&p_function, _stop, &_cond, &_ret]() -> void {
            _ret = p_function(_stop);
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return {_ret};
        }

        *_stop = true;
        _th.join();
        return {};
    }
};

template <>
struct executer_t<void> {
    template <typename t_time, typename t_function, typename... t_params>
    bool operator()(t_time p_timeout,
                    t_function p_function,
                    std::tuple<t_params...> &&p_tuple) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        std::tuple<ptr<bool>, t_params...> _tuple =
            std::tuple_cat(std::make_tuple(_stop), std::move(p_tuple));

        std::thread _th([&p_function, &_cond, &_tuple]() -> void {
            std::apply(p_function, std::move(_tuple));
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return true;
        }

        *_stop = true;
        _th.join();
        return false;
    }

    template <typename t_time, typename t_function, typename t_param>
    bool
    operator()(t_time p_timeout, t_function p_function, t_param &&p_param) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        DEB("param = ", p_param);

        std::thread _th(
            [this, &p_function, &_stop, &_cond, &p_param]() -> void {
                DEB("about to call function for ", p_param);
                p_function(_stop, std::move(p_param));
                DEB("about to notify");
                _cond.notify_one();
            });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            DEB("no timeout");
            _th.join();
            DEB("returning true");
            return true;
        }

        DEB("timeout, setting p_stop to true");
        *_stop = true;
        _th.join();
        DEB("returning false");
        return false;
    }

    template <typename t_time, typename t_function>
    bool operator()(t_time p_timeout, t_function p_function) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        std::thread _th([&p_function, _stop, &_cond]() -> void {
            DEB("about to execute");
            p_function(_stop);
            DEB("execution returned");
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return true;
        }

        *_stop = true;
        _th.join();
        return false;
    }
};

template <typename t_time, typename t_function, typename... t_params>
typename executer_traits_t<
    std::invoke_result_t<t_function, ptr<bool>, t_params...>>::result
execute(t_time p_timeout,
        t_function p_function,
        std::tuple<t_params...> &&p_tuple) {
    typedef typename executer_traits_t<
        std::invoke_result_t<t_function, ptr<bool>, t_params...>>::type type;

    executer_t<type> _executer;

    return _executer(p_timeout, p_function, std::move(p_tuple));
}

template <typename t_time, typename t_function, typename t_param>
typename executer_traits_t<
    std::invoke_result_t<t_function, ptr<bool>, t_param>>::result
execute(t_time p_timeout, t_function p_function, t_param &&p_param) {
    typedef typename executer_traits_t<
        std::invoke_result_t<t_function, ptr<bool>, t_param>>::type type;

    executer_t<type> _executer;

    return _executer(p_timeout, p_function, std::move(p_param));
}

template <typename t_time, typename t_function>
typename executer_traits_t<std::invoke_result_t<t_function, ptr<bool>>>::result
execute(t_time p_timeout, t_function p_function) {
    typedef typename executer_traits_t<
        std::invoke_result_t<t_function, ptr<bool>>>::type type;

    executer_t<type> _executer;

    return _executer(p_timeout, p_function);
}

/// \brief
template <typename t_data>
struct queue_t {
    virtual ~queue_t() {}

    queue_t(const queue_t &) = default;
    queue_t(queue_t &&) = default;
    queue_t &operator=(const queue_t &) = default;
    queue_t &operator=(queue_t &&) = default;

    virtual void add(const t_data &) = 0;
    virtual void add(t_data &&p_data) = 0;

    virtual std::pair<bool, t_data> get() = 0;

    virtual void traverse(std ::function<void(const t_data &)>) const = 0;
    virtual bool full() = 0;
    virtual bool empty() = 0;
    virtual size_t capacity() const = 0;
    virtual size_t occupied() const = 0;

protected:
    queue_t(size_t p_size = 0)
        : m_size(p_size) {}

protected:
    size_t m_size {0};
};

/// \brief Implements a circular queue which size is increased if it
/// becomes full
///
/// \tparam t_data defines the types of the data contained in the
/// buffer
template <typename t_data>
struct circular_unlimited_size_queue_t : public internal::queue_t<t_data> {
    /// \brief Constructor
    ///
    /// \param p_size the number of initial slots in the queue
    circular_unlimited_size_queue_t(size_t p_size = 1)
        : internal::queue_t<t_data>() {
        m_root = create_node();
        node_ptr _p = m_root;
        for (size_t _i = 0; _i < p_size; ++_i) {
            _p = insert(_p, t_data());
        }
        //    this->m_size = p_size;
        m_write = m_root;
        m_read = m_root;
    }

    ~circular_unlimited_size_queue_t() override {}

    /// \brief Copy constructor not allowed
    circular_unlimited_size_queue_t(const circular_unlimited_size_queue_t &) =
        delete;

    /// \brief Move constructor not allowed
    circular_unlimited_size_queue_t(circular_unlimited_size_queue_t &&p_queue) {
        m_root = p_queue.m_root;
        m_write = p_queue.m_write;
        m_read = p_queue.m_read;
        m_amount = p_queue.m_amount;
    }

    /// \brief Copy assignment not allowed
    circular_unlimited_size_queue_t &
    operator=(const circular_unlimited_size_queue_t &) = delete;

    /// \brief Move assignment not allowed
    circular_unlimited_size_queue_t &
    operator=(circular_unlimited_size_queue_t &&p_queue) {
        if (this != p_queue) {
            m_root = p_queue.m_root;
            m_write = p_queue.m_write;
            m_read = p_queue.m_read;
            m_amount = p_queue.m_amount;
        }
        return *this;
    }

    /// \brief Traverses the queue
    ///
    /// \param p_function will be called for every data in the queue
    void
    traverse(std::function<void(const t_data &)> p_visitor) const override {
        node_ptr _p = m_root;
        while (_p && (_p->m_next != m_root)) {
            p_visitor(_p->m_data);
            _p = _p->m_next;
        }
        p_visitor(_p->m_data);
    }

    /// \brief Adds a t_data object to the queue
    ///
    /// \param p_data is a t_data to be added
    void add(const t_data &p_data) override {
        DEB("adding ", p_data);
        {
            std::lock_guard<std::mutex> _lock(m_mutex);
            bool _full = full();
            if (!_full) {
                m_write->m_data = p_data;
                m_write = m_write->m_next;
            } else {
                insert(m_write->m_prev, p_data);
            }
            ++m_amount;
        }
        DEB("capacity = ", capacity(), ", occupied = ", occupied());
    }

    /// \brief Moves a t_data object to the queue
    ///
    /// \param p_data is a t_data to be added
    void add(t_data &&p_data) override {
        DEB("adding ", p_data);
        {
            std::lock_guard<std::mutex> _lock(m_mutex);
            bool _full = full();
            if (!_full) {
                m_write->m_data = p_data;
                m_write = m_write->m_next;
            } else {
                insert(m_write->m_prev, std::move(p_data));
            }
            ++m_amount;
        }
        DEB("capacity = ", capacity(), ", occupied = ", occupied());
    }

    std::pair<bool, t_data> get() override {
        std::lock_guard<std::mutex> _lock(m_mutex);
        if (empty()) {
            return {false, t_data()};
        }

        t_data _data(m_read->m_data);
        m_read = m_read->m_next;
        --m_amount;
        return {true, std::move(_data)};
    }

    /// \brief Informs if the queue is full
    inline bool full() override {
        //        std::lock_guard<std::mutex> _lock(m_mutex);
        return m_amount == this->m_size;
    }

    /// \brief Informs if the queue is empty
    inline bool empty() override {
        //        std::lock_guard<std::mutex> _lock(m_mutex);
        return m_amount == 0;
    }

    /// \brief Informs the total capacity of the queue
    inline size_t capacity() const override { return this->m_size; }

    /// \brief Informs the current number of slots occupied in the
    /// queue
    inline size_t occupied() const override { return m_amount; }

private:
    /// \brief Node of the linked list used to implement the queue
    struct node {
        /// \brief Type of pointer
        typedef std::shared_ptr<node> ptr;

        node() = default;

        /// \brief Copy constructor not allowed
        node(const node &) = delete;

        node(node &&) = default;

        /// \brief Copy assignment not allowed
        node &operator=(const node &) = delete;

        node &operator=(node &&) = default;

        /// \brief Constructor
        ///
        /// \param p_data is a t_data to be moved into the node
        node(t_data &&p_data)
            : m_data(std::move(p_data)) {}

        /// \brief Constructor
        ///
        /// \param p_data is a t_data to be copied into the node
        node(const t_data &p_data)
            : m_data(p_data) {}

        /// \brief data in the node
        t_data m_data;

        /// \brief next node
        ptr m_next;

        /// \brief previous node
        ptr m_prev;
    };

    /// \brief Alias for the pointer to a node
    typedef typename node::ptr node_ptr;

private:
    /// \brief Inserts a node in the list after a node
    ///
    /// \param p_node which the new node will be inserted in front
    /// of
    ///
    /// \param p_data data inserted in the new node
    ///
    /// \return the new node
    node_ptr insert(node_ptr p_node, const t_data &p_data) {
        node_ptr _new_node = create_node(std::move(p_data));

        _new_node->m_next = p_node->m_next;
        _new_node->m_prev = p_node;

        p_node->m_next->m_prev = _new_node;
        p_node->m_next = _new_node;
        ++this->m_size;
        return _new_node;
    }

    /// \brief Inserts a node in the list after a node
    ///
    /// \param p_node which the new node will be inserted in front
    /// of
    ///
    /// \param p_data data inserted in the new node
    ///
    /// \return the new node
    node_ptr insert(node_ptr p_node, t_data &&p_data) {
        node_ptr _new_node = create_node(std::move(std::move(p_data)));

        _new_node->m_next = p_node->m_next;
        _new_node->m_prev = p_node;

        p_node->m_next->m_prev = _new_node;
        p_node->m_next = _new_node;
        ++this->m_size;
        return _new_node;
    }

    /// \brief Creates a new node
    ///
    /// \return the new node
    node_ptr create_node() {
        node_ptr _p(std::make_shared<node>());
        _p->m_next = _p;
        _p->m_prev = _p;
        return _p;
    }

    /// \brief Creates a new node, defining its data
    ///
    /// \param p_data is the data inside the new node
    ///
    /// \return the new node
    node_ptr create_node(const t_data &p_data) {
        node_ptr _p(std::make_shared<node>(p_data));
        _p->m_next = _p;
        _p->m_prev = _p;
        return _p;
    }

    /// \brief Creates a new node, defining its data
    ///
    /// \param p_data is the data inside the new node
    ///
    /// \return the new node
    node_ptr create_node(t_data &&p_data) {
        node_ptr _p(std::make_shared<node>(std::move(p_data)));
        _p->m_next = _p;
        _p->m_prev = _p;
        return _p;
    }

private:
    /// \brief The first node of the queue
    node_ptr m_root;

    /// \brief The node where the next write, i.e., new data
    /// insertion, should be done
    node_ptr m_write;

    /// \brief The node where the next read, i.e., new data
    /// extraction, should be done
    node_ptr m_read;

    /// \brief Amount of nodes actually used
    size_t m_amount {0};

    /// \brief Controls insertion
    std::mutex m_mutex;
};

/// \brief A group of functions that compete to handle a data added
/// to a queue
///
/// \param t_data type of data to be processed
template <typename t_event>
class handling_t {
public:
    /// \brief Type of function that will handle the data
    typedef std::function<void(ptr<bool>, t_event &&)> handler;

public:
    /// \brief Default constructor not allowed
    handling_t() = delete;

    /// \brief Constructor
    /// Using this constructor, this group of handlers will have the
    /// tenacitas::async::priority::lowest priority
    ///
    /// \tparam t_time is the type of time used to define the
    /// timeout for all the handler functions. It must be one of the
    /// defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_id identifier of this group of handlers
    ///
    /// \param p_timeout is the value of timeout for all the handler
    /// functions
    ///
    template <typename t_time>
    handling_t(const number::id &p_owner,
               t_time p_timeout,
               priority p_priority = 125)
        : m_owner(p_owner)
        , m_timeout(calendar::convert<timeout>(p_timeout))
        , m_priority(p_priority)
        , m_queue(1) {}

    /// \brief Copy constructor not allowed
    handling_t(const handling_t &) = delete;

    handling_t(handling_t &&) = delete;

    /// \brief Copy assignemnt not allowed
    handling_t &operator=(const handling_t &) = delete;

    handling_t &operator=(handling_t &&) = delete;

    ~handling_t() {
        DEB(m_owner, ':', m_id, " - entering");
        stop();
        DEB(m_owner, ':', m_id, " - leaving");
    }

    /// \brief Adds data to be passed to a handler
    ///
    /// \param p_data is the data to be passed to a worker
    ///
    /// \return \p true if it was added, \p false otherwise
    bool add_data(const t_event &p_data) {
        DEB(m_owner, ':', m_id, " - adding ", p_data);
        m_queue.add(p_data);
        m_cond.notify_all();
        {
            std::lock_guard<std::mutex> _lock(m_mutex);
            ++m_queued_data;
        }
        return true;
    }

    /// \return the amount of data added
    inline size_t amount_added() const { return m_queue.capacity(); }

    /// \brief Adds one handler
    ///
    /// \tparam t_time type of time used to define timeout for the
    /// worker
    ///
    /// \param p_function the \p worker to be added
    void add_handler(handler &&p_handler) {
        std::lock_guard<std::mutex> _lock(m_add_handler);

        m_loops.push_back(std::thread(&handling_t<t_event>::handler_loop, this,
                                      std::move(p_handler)));
        m_stopped = false;
    }

    /// \brief Adds a bunch of handler function to a group of
    /// handlers
    ///
    /// \param p_num_workers defines the number of handler functions
    /// to be added
    ///
    /// \param p_factory is a function that creates handler function
    void add_handler(uint16_t p_num_workers,
                     std::function<handler()> p_factory) {
        for (uint16_t _i = 0; _i < p_num_workers; ++_i) {
            add_handler(p_factory());
        }
    }

    /// \return Informs if the worker is stopped
    inline bool is_stopped() const { return m_stopped; }

    /// \return The capacity if the queue
    inline size_t capacity() const { return m_queue.capacity(); }

    /// \return The amount of slots occupied in the \p queue
    inline size_t occupied() const { return m_queue.occupied(); }

    /// \return The timeout defined for this group of handlers
    template <typename t_time>
    inline t_time get_timeout() const {
        return calendar::convert<t_time>(m_timeout);
    }

    /// \return The tenacitas::async::id of this group of handlers
    inline const number::id &get_owner() const { return m_owner; }

    /// \return The tenacitas::async::id of this group of handlers
    inline const number::id &get_id() const { return m_id; }

    /// \return The tenacitas::async::priority of this group of
    /// handlers
    inline const priority &get_priority() const { return m_priority; }

    /// \brief Sets the tenacitas::async::priority of this group of
    /// handlers
    inline void set_priority(priority p_priority) { m_priority = p_priority; }

    /// \return Returns the size of the queue of \p t_data
    size_t get_size() const { return m_queue.capacity(); }

    /// \return Returns the amount of \p t_data objects in the queue
    size_t get_occupied() const { return m_queue.occupied(); }

    /// \brief Less-than
    /// \p handlers are ordered by tenacitas::async::priority
    inline bool operator<(const handling_t &p_handlers) const {
        return m_priority < p_handlers.m_priority;
    }

    /// \brief Greater-than
    /// \p handlers are ordered by tenacitas::async::priority
    inline bool operator>(const handling_t &p_handlers) const {
        return m_priority > p_handlers.m_priority;
    }

    /// \brief Not-equal
    /// \p handlers are compared by tenacitas::async::id
    inline bool operator!=(const handling_t &p_handlers) const {
        return m_id != p_handlers.m_id;
    }

    /// \brief Equal-to
    /// \p handlers are compared by tenacitas::async::id
    inline bool operator==(const handling_t &p_handlers) const {
        return m_id == p_handlers.m_id;
    }

private:
    /// \brief Type of group of loops
    typedef typename std::vector<std::thread> loops;

    /// \brief Type of the queue used to store the data to be
    /// handled
    typedef circular_unlimited_size_queue_t<t_event> queue;

private:
    void handler_loop(handler &&p_handler) {
        number::id _loop_id;
        DEB(m_owner, ':', m_id, ':', _loop_id,
            " - entering loop, m_timeout = ", this->m_timeout.count());

        while (true) {
            {
                std::unique_lock<std::mutex> _lock(m_mutex);
                m_cond.wait(_lock, [this, _loop_id]() -> bool {
                    if (m_stopped) {
                        DEB(m_owner, ':', m_id, ':', _loop_id, " - stopped");
                        return true;
                    }
                    if (!m_queue.empty()) {
                        DEB(m_owner, ':', m_id, ':', _loop_id,
                            " - there is data");
                        return true;
                    }
                    DEB(m_owner, ':', m_id, ':', _loop_id, " - waiting");
                    return false;
                });
            }

            if (m_stopped) {
                DEB(m_owner, ':', m_id, ':', _loop_id, " - stop");
                break;
            }

            //            std::optional<t_data> _maybe_data {m_queue.template
            //            get<t_data>()};

            std::pair<bool, t_event> _maybe_data {m_queue.get()};

            if (this->m_stopped) {
                DEB(m_owner, ':', m_id, ':', _loop_id, " - stop");
                break;
            }

            if (_maybe_data.first) {
                t_event _data {std::move(_maybe_data.second)};

                DEB(m_owner, ':', m_id, ':', _loop_id, " - data = ", _data);

                if (this->m_stopped) {
                    DEB(m_owner, ':', m_id, ':', _loop_id, " - stop");
                    break;
                }

                if (!execute(m_timeout, p_handler, std::move(_data))) {
                    WAR(m_owner, ':', m_id, ':', _loop_id, " - timeout ",
                        _data);
                }
            } else {
                DEB(m_owner, ':', m_id, ':', _loop_id, " - no data available");
            }
        }
    }

    /// \brief
    inline void empty_queue() {
        DEB(this->m_owner, ':', this->m_id, " - empty queue");
        while (!m_queue.empty()) {
            m_cond.notify_all();
        }
    }

    /// \brief Sets this group of handles to stop
    inline void stop() {
        if (m_stopped) {
            DEB(m_owner, ':', m_id, " - not stopping because it is stopped");
            return;
        }

        DEB(m_owner, ':', m_id, " - stoping");

        //      std::unique_lock<std::mutex> _lock(m_mutex_stop);
        m_stopped = true;
        m_cond.notify_all();
        for (std::thread &_thread : m_loops) {
            if (_thread.joinable()) {
                _thread.join();
            }
        }
        m_stopped = true;
    }

private:
    number::id m_owner;

    /// \brief Maximum amount of time a handler has to complete
    timeout m_timeout;

    /// \brief Priority of this group of handlers
    priority m_priority;

    /// \brief Queue where data will be inserted for the handlers to
    /// compete for handling
    queue m_queue;

    /// \brief Identifier of this group of handlers
    number::id m_id;

    /// \brief Asynchronous loops, where the handlers are running
    loops m_loops;

    /// \brief Indicates if the worker is running
    std::atomic<bool> m_stopped {true};

    /// \brief Amount of queued data
    size_t m_queued_data {0};

    /// \brief Controls access to the \p m_loops while inserting a
    /// new \p handler
    std::mutex m_add_handler;

    /// \brief Controls access to attributes while the worker is
    /// stopping
    //  std::mutex m_mutex_stop;

    /// \brief Controls access to inserting data
    std::mutex m_mutex;

    /// \brief Controls access to the data produced
    std::condition_variable m_cond;
};

template <typename t_event>
struct event_id_t {
    static number::id value;
};

template <typename t_event>
number::id event_id_t<t_event>::value;

/// \brief Distributes a event to a list of group of handlers
/// Each group of handlers handles the same event in a different
/// way, has its own definition of timeout for the handlers, its own
/// definition of priority, and what should be done if a handler
/// times out
///
/// \tparam is the type of event this dispatcher distributes
template <typename t_event>
struct dispatcher_t {
    /// \brief Type of handler
    typedef std::function<void(ptr<bool>, t_event &&)> handler;

    ~dispatcher_t() = default;

    /// \brief Adds a group of handlers to the dispatcher, which will
    /// handle a event in a specific way
    ///
    /// \tparam t_time is the type of time used to define the
    /// timeout for all the handler functions. It must be one of the
    /// defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_id identifier of this group of handlers
    ///
    /// \param p_priority is the priority of this group of handlers
    ///
    /// \param p_timeout is the value of timeout for all the handler
    /// functions
    template <typename t_time>
    static inline number::id add_handling(const t_time &p_timeout,
                                          priority p_priority = 125) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_list.push_back(std::make_unique<handling>(event_id_t<t_event>::value,
                                                    p_timeout, p_priority));
        number::id _id = m_list.back()->get_id();
        //    DEB(evt_id_t<t_event>::value)
        sort();
        return _id;
    }

    /// \brief Adds a group of handlers to the dispatcher, which will
    /// handle a event in a specific way, and adds a handler
    /// function to the group of handlers
    ///
    /// \tparam t_time is the type of time used to define the
    /// timeout for all the handler functions. It must be one of the
    /// defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_handler is the handler function to be added
    ///
    /// \param p_timeout is the value of timeout for all the handler
    /// functions
    ///
    /// \param p_priority is the priority of this group of handlers
    ///
    /// \return p_id identifier of this group of handlers
    template <typename t_time>
    static number::id add_handling(handler &&p_handler,
                                   t_time p_timeout,
                                   priority p_priority = 125) {
        number::id _id = add_handling(p_timeout, p_priority);
        add_handler(_id, std::move(p_handler));
        return _id;
    }

    /// \brief Sets the priority for a group of handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \param p_priority is the priority to be set for the group of
    /// handlers
    static void set_priority(const number::id &p_id, priority p_priority) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            (*_ite)->set_priority(p_priority);
            sort();
        }
    }

    /// \brief Retrieves the priority for a group of handlers, if
    /// found
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \return the priority of the group of handlers, if \p p_id
    /// exists
    static std::optional<priority> get_priority(const number::id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return {(*_ite)->get_priority()};
        }
        return {};
    }

    /// \brief Sends a data to be handled
    /// This data will be copied to all the handler groups, and one
    /// of the handler functions in each handler group will handle
    /// the event
    ///
    /// \param p_event is the event to be handled
    static void send(const t_event &p_event) {
        for (handling_ptr &_handling_ptr : m_list) {
            INF(get_id(), " - sending ", p_event, " to pool ",
                _handling_ptr->get_id());
            _handling_ptr->add_data(p_event);
        }
    }

    /// \brief Adds a handler function to a group of handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \param p_handler is the handler function to be added
    static void add_handler(const number::id &p_handling_id,
                            handler &&p_handler) {
        auto _handling_ite = find(p_handling_id);
        auto _end = m_list.end();
        if (_handling_ite != _end) {
            DEB(get_id(), " - adding another handler for ", p_handling_id);
            (*_handling_ite)->add_handler(std::move(p_handler));
        }
    }

    /// \brief Adds a bunch of handler function to a group of
    /// handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \param p_num_workers defines the number of handler functions
    /// to be added
    ///
    /// \param p_factory is a function that creates handler function
    static void add_handler(const number::id &p_handling_id,
                            uint16_t p_num_workers,
                            std::function<handler()> p_factory) {
        iterator _handling_ite = find(p_handling_id);
        if (_handling_ite != m_list.end()) {
            (*_handling_ite)->add_handler(p_num_workers, p_factory);
        }
    }

    /// \brief Traverse the groups of handlers
    ///
    /// \param p_visitor is a function that will be called for each
    /// group of handlers.
    static void
    traverse(std::function<void(const number::id &,
                                priority,
                                const std::chrono::milliseconds &)> p_visitor) {
        for (const handling_ptr &_handling_ptr : m_list) {
            p_visitor(_handling_ptr->get_id(), _handling_ptr->get_priority(),
                      _handling_ptr
                          ->template get_timeout<std::chrono::milliseconds>());
        }
    }

    /// \brief Retrieves the size of the queue of events for a
    /// group of handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ////
    /// \return the size of the event queue
    static size_t size(const number::id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return (*_ite)->get_size();
        }
        return 0;
    }

    /// \brief Retrieves how many positions in the queue of events
    /// for a group of handlers are occupied
    ///
    /// \param p_id is the identifier of the group of handlers
    ////
    /// \return the number of occupied positions
    static size_t occupied(const number::id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return (*_ite)->get_occupied();
        }
        return 0;
    }

    /// \brief Waits for all the events in all the groups of
    /// handlers to be handled
    static void wait() {
        DEB(event_id_t<t_event>::value, "  - starting to wait");
        for (handling_ptr &_handling_ptr : m_list) {
            _handling_ptr->empty_queue();
        }
        DEB(event_id_t<t_event>::value, "  - finished waiting");
    }

    static number::id get_id() { return event_id_t<t_event>::value; }

private:
    /// \brief Alias for a handling for this event
    typedef internal::handling_t<t_event> handling;

    typedef std::unique_ptr<handling> handling_ptr;

    /// \brief List of handlings
    typedef std::list<handling_ptr> handling_list;

    /// \brief Iterator for the list of handlers
    typedef typename handling_list::iterator iterator;

private:
    /// \brief Finds a group of handlers based on a
    /// tenacitas::async::id
    ///
    /// \return an iterator to the group of handlers, of
    /// m_list.end() if not
    static iterator find(const number::id &p_id) {
        auto _cmp = [&p_id](const handling_ptr &p_handlers) -> bool {
            return p_id == p_handlers->get_id();
        };
        return std::find_if(m_list.begin(), m_list.end(), _cmp);
    }

    /// \brief Inserts a group of handlers to the list
    ///
    /// \param p_handlers is the group of handlers to be added
    static inline void insert(handling_ptr &&p_handlers) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_list.push_back(std::move(p_handlers));
        //    m_list.back().start();
        sort();
    }

    /// \brief Sorts the list of group of handlers
    static inline void sort() {
        //    std::sort(m_list.begin(), m_list.end());
        m_list.sort(
            [](const handling_ptr &p_i1, const handling_ptr &p_i2) -> bool {
                return (*p_i1) < (*p_i2);
            });
    }

private:
    /// \brief The list of group of handlers
    static handling_list m_list;

    /// \brief Access control
    static std::mutex m_mutex;
};

/// \brief
template <typename t_data>
typename dispatcher_t<t_data>::handling_list dispatcher_t<t_data>::m_list;

template <typename t_data>
std::mutex dispatcher_t<t_data>::m_mutex;

} // namespace internal

struct sleeping_loop {

    /// \brief Signature of the function that will be called in each round of
    /// the loop
    typedef std::function<void(ptr<bool>)> function;

    sleeping_loop() = delete;

    sleeping_loop(const sleeping_loop &) = delete;
    sleeping_loop &operator=(const sleeping_loop &) = delete;
    ~sleeping_loop() { stop(); }

    sleeping_loop(sleeping_loop &&p_loop) {
        bool _stopped = p_loop.m_stopped;

        m_owner = std::move(p_loop.m_owner);
        m_function = p_loop.m_function;
        m_timeout = std::move(p_loop.m_timeout);
        m_interval = std::move(p_loop.m_interval);
        m_stopped = true;

        if (!_stopped) {
            DEB("right side not stopped");
            p_loop.stop();
            start();
        }
    }

    sleeping_loop &operator=(sleeping_loop &&p_loop) {
        if (this != &p_loop) {

            bool _stopped = p_loop.m_stopped;

            m_owner = std::move(p_loop.m_owner);
            m_function = p_loop.m_function;
            m_timeout = std::move(p_loop.m_timeout);
            m_interval = std::move(p_loop.m_interval);
            m_stopped = true;

            if (!_stopped) {
                DEB("right side not stopped");
                p_loop.stop();
                start();
            }
        }
        return *this;
    }

    template <typename t_timeout, typename t_interval>
    sleeping_loop(const number::id &p_owner,
                  function p_function,
                  t_timeout p_timeout,
                  t_interval p_interval)
        : m_owner(p_owner)
        , m_function(p_function)
        , m_timeout(calendar::convert<internal::timeout>(p_timeout))
        , m_interval(calendar::convert<internal::interval>(p_interval)) {}

    void start() {
        if (!m_stopped) {
            return;
        }
        m_stopped = false;

        DEB(m_owner, ':', m_id, " - starting loop thread");
        m_thread = std::thread([this]() { loop(); });
    }

    /// \brief Stops the loop, if it was started
    void stop() {

        if (m_stopped) {
            DEB(m_owner, ':', m_id, " - not stopping because it is stopped");
            return;
        }
        DEB(m_owner, ':', m_id, " - stopping");
        m_stopped = true;
        m_cond.notify_one();

        if (m_thread.get_id() == std::thread::id()) {
            DEB(m_owner, ':', m_id,
                " - not joining because m_thread.get_id() == "
                "std::thread::id()");
            return;
        }
        DEB(m_owner, ':', m_id, " - m_thread.get_id() != std::thread::id()");

        if (!m_thread.joinable()) {
            DEB(m_owner, ':', m_id,
                " - not joining because m_thread is not joinable");
            return;
        }
        DEB(m_owner, ':', m_id, " - m_thread.joinable()");

        {
            std::lock_guard<std::mutex> _lock(m_mutex_join);
            m_thread.join();
        }
        DEB(m_owner, ':', m_id, " - leaving stop");
    }

    inline bool is_stopped() const { return m_stopped; }

private:
    void loop() {
        DEB(m_owner, ':', m_id,
            " - entering loop, m_timeout = ", m_timeout.count());

        while (true) {
            if (m_stopped) {
                DEB(m_owner, ':', m_id, " - stop");
                break;
            }

            DEB(m_owner, ':', m_id, " - calling handler");

            if (!internal::execute(m_timeout, [this](ptr<bool> p_stop) -> void {
                    m_function(p_stop);
                })) {
                WAR(m_owner, ':', m_id, " - timeout");
            }

            if (m_stopped) {
                DEB(m_owner, ':', m_id, " - stop");
                break;
            }

            DEB(m_owner, ':', m_id, " - waiting for ", m_interval.count(),
                "ms to elaps, or a stop order");
            {
                std::unique_lock<std::mutex> _lock(m_mutex_interval);
                if ((m_cond.wait_for(_lock, m_interval) ==
                     std::cv_status::no_timeout) &&
                    (m_stopped)) {
                    DEB(m_owner, ':', m_id, " - ordered to stop");
                    break;
                }
            }
            DEB(m_owner, ':', m_id, " - ", m_interval.count(), "ms elapsed");
        }
        DEB(m_owner, ':', m_id, " - leaving loop");
    }

private:
    /// \brief Identifier of the object that owns this object
    number::id m_owner;

    /// \brief Function that will be called in each round of the loop
    function m_function;

    /// \brief Maximum time that \p m_function will have to execute
    internal::timeout m_timeout;

    internal::interval m_interval;

    /// \brief Identifier of this object, used for debugging purposes
    number::id m_id;

    /// \brief Indicates that the loop must stop
    std::atomic<bool> m_stopped {true};

    /// \brief Thread where the \p loop method will run
    std::thread m_thread;

    /// \brief Protects joining the thread
    std::mutex m_mutex_join;

    std::mutex m_mutex_interval;

    std::condition_variable m_cond;

    //  std::mutex m_mutex_stop;
};

// #########################################

template <typename t_event>
using handler_t = std::function<void(ptr<bool>, t_event &&)>;

/// \brief Adds a queue to receive events to be handled
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order hat
/// the event will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
template <typename t_event, typename t_time>
static inline number::id add_handling(const t_time &p_timeout,
                                      priority p_priority = 125) {
    return internal::dispatcher_t<t_event>::add_handling(p_timeout, p_priority);
}

/// \brief Adds a queue to receive events to be handled
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_handler is the handler function to be added
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order hat
/// the event will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
template <typename t_event, typename t_time>
static inline number::id add_handling(handler_t<t_event> &&p_handler,
                                      const t_time &p_timeout,
                                      priority p_priority = 125) {
    return internal::dispatcher_t<t_event>::add_handling(std::move(p_handler),
                                                         p_timeout, p_priority);
}

/// \brief Defines the priority of a event queue
/// The priority of the queue defines the order in which a event will be
/// added.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the event will be placed in the queue.
template <typename t_event>
static inline void set_priority(const number::id &p_handlers,
                                priority p_priority) {
    internal::dispatcher_t<t_event>::set_priority(p_handlers, p_priority);
}

/// \brief Retrieves the priority of a event queue
/// The priority of the queue defines the order in which a event will be
/// added.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_id is the identifier of the queue
///
/// \return The priority of the queue
template <typename t_event>
static inline std::optional<priority>
get_priority(const number::id &p_handlers) {
    return internal::dispatcher_t<t_event>::get_priority(p_handlers);
}

/// \brief Sends a event to all the queues associated to a event type
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_event is the event to be copied to all the queues
template <typename t_event>
static inline void dispatch(const t_event &p_event) {
    internal::dispatcher_t<t_event>::send(p_event);
}

/// \brief Adds a handler to a queue
/// It is possible to add many handlers to the same queue. Those handlers will
/// compete with each other to handle a added event.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_id is the identifier of the queue to which this handler will be
/// added to
///
/// \param p_handler is the function that will handle a event added to the
/// queue
template <typename t_event>
static inline void add_handler(const number::id &p_handlers,
                               handler_t<t_event> &&p_handler) {
    internal::dispatcher_t<t_event>::add_handler(p_handlers,
                                                 std::move(p_handler));
}

template <typename t_event>
static inline void add_handler(const number::id &p_handling,
                               uint16_t p_num_workers,
                               std::function<handler_t<t_event>()> p_factory) {
    internal::dispatcher_t<t_event>::add_handler(p_handling, p_num_workers,
                                                 p_factory);
}

template <typename t_event, typename t_time>
static inline const number::id add_handler(handler_t<t_event> &&p_handler,
                                           t_time p_timeout,
                                           priority p_priority = 125) {
    number::id _id = add_handling<t_event>(p_timeout, p_priority);
    internal::dispatcher_t<t_event>::add_handler(_id, std::move(p_handler));
    return _id;
}

/// \brief Traverse the groups of handlers
///
/// \param p_visitor is a function that will be called for each group of
/// handlers. It must implement \code template<typename t_time> void
/// operator()(const number::id&, const priority &, const t_time&) \endcode
template <typename t_event>
static inline void
traverse(std::function<void(const number::id &,
                            const priority &,
                            const std::chrono::milliseconds &)> p_visitor) {
    internal::dispatcher_t<t_event>::traverse(p_visitor);
}

} // namespace async
} // namespace tenacitas

#endif
