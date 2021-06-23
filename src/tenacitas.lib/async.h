#ifndef TENACITAS_ASYNC_H
#define TENACITAS_ASYNC_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher_000/main.cpp
/// \example dispatcher_001/main.cpp
/// \example dispatcher_002/main.cpp
/// \example sleeping_loop_000/main.cpp
/// \example executer_000/main.cpp

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
#include <utility>
#include <vector>

#include <tenacitas.lib/calendar.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/number.h>
#include <tenacitas.lib/type.h>

using namespace std::chrono_literals;

/// \brief master namespace
namespace tenacitas {

/** \brief support for async (asynchronous) programming
 *
 *The UML below shows the basic structure of the \p async namespace
 *\startuml
 *hide empty members
 *allow_mixing
 *skinparam linetype ortho
 *hide empty members
 *allow_mixing
 *skinparam linetype ortho
 *
 *
 *class handling_t <t_event>
 *class handler_t <t_event>
 *class dispatcher_t <t_event>
 *
 *dispatcher_t *-- "*" handling_t
 *handling_t *-- "*" handler_t
 *
 *handling_t *-- timeout
 *handling_t *-- priority
 *\enduml
 *
 *The central concept is an \p t_event. A \p t_event is a struct that contains
 *data about an interesting event in the application. It can be, for example, a
 *incoming message from a network connection, or a user menu choice.
 *
 *A \p handler_t is a function class that will handle a \p t_event. We can see
 *in the \p tenacitas::async::handler_t signature that it also receives a
 *<tt>std::share_ptr<bool></tt> object. Through this pointer a \p handler_t
 *function may be informed that its execution took more time than specified, so
 *that it can make the decision to stop running.
 *
 *A \p handling_t defines a type of handling for a \p t_event, and can group as
 *many \p handler_t objects as necessary. For example, a user menu choice can
 *generate a log message, a message sent to a remote program, and to change the
 *color of widget in the user interface. Each one of this actions is a \p
 *handling_t. Besides, it is possible to define one handler for the logging
 *handling, five handlers to the network message handling, and two for the
 *widget color changing. When the user makes its menu choice, the five \p
 *handler_t objects in the message network \p handling_t will "fight each other"
 *to get the \p t_event to handle.
 *
 *Each \p handling_t defines a timeout applies to all its \p handler_t objects,
 *and it is possible to define a tenacitas::async::priority for a \p handling_t,
 *so that the higher priority will receive a \p t_event before the others \p
 *handling_t.
 *
 *The \p dispatcher_t class is responsible for managing the creation of \p
 *handling_t objects, adding \p handler_t objects to a \p handling_t, and
 *dispatching \p t_event objects to the \p handing_t.
 *
 *In order to make it easier to deal with these classes, the \p async namespace
 *implements a set of functions: tenacitas::async::add_handling,
 *tenacitas::async::add_handler, tenacitas::async::set_priority and
 *tenacitas::async::dispatch, with some variations.
 *
 *The \p sleeping_loop_t allows a function to be executed in a defined
 *interval. It is usefull, among others, to dispatch \p t_event, in order to
 *simulate an actual event generation and handling.
 *
 *Please, look at the \p Examples section for examples on how to use these
 *functions and classes.
 *

 */
namespace async {

// for type::ptr
using namespace tenacitas::type;

/// \brief Defines the priority of a handling for an event among other handlings
/// of that event
typedef uint8_t priority;

/// \brief Identifier of a handling
/// It is simply a, hopefully, unique number
typedef number::id handling_id;

/// \brief Type of function that handles an event
///
/// \tparam t_event is the type of event that will be handled
/// It must be default constructible, copy constructible, copy assignable, move
/// constructible, move assignable and must have the output operator (<<)
/// defined
///
/// \param p_bool is a means for the object that will call the handler function
/// to inform that the handler may stop working, as the timeout defined for it
/// to finish its work has expired. So, the handler function should check,
/// between its instructions, for the value o \p p_bool, in order to
/// continue to the following instruction, or to interrupt its execution
///
/// \param p_event is the event to be handled
template <typename t_event>
using handler_t = std::function<void(ptr<bool> p_bool, t_event &&p_event)>;

// \brief internal classes, objects and function
namespace internal {

// \brief Type of time used to define timeout
typedef std::chrono::milliseconds timeout;

// \brief Type of time used to define interval
typedef std::chrono::milliseconds interval;

/// \brief Synchronous execution, but with timeout control
///
/// \tparam t_ret type that the function to be executed returns
///
/// \tparam t_time type of the time used to specify timeout
///
/// \tparam t_function function to be executed. It must have this signature:
/// <tt>std::function<t_ret(tenacitas::type::ptr<bool> p_bool, t_params...
/// p_params)></tt>. The \p p_bool is how \p execute will let the function know
/// that it has exceed the time limit for its execution. The function should
/// between its instructions to check for \p p_bool, to check if it should stop
/// executing.
///
/// \tparam t_params... type of the parameters to the function
///
/// \param p_timeout maximum amount of time that the function has to finish its
/// execution. It must be one of the types defined in <tt>std::chrono</tt>
///
/// \param p_function function to execute in \p p_timeout time, with the
/// signature described above.
///
/// \param p_param... arguments to \p p_function
///
/// \return if \p p_function executed in less than \p p_timeout, a valid \p
/// t_ret value; or an empty std::optional, otherwise.

template <typename t_ret>
struct executer_t {
    template <typename t_time, typename t_function, typename... t_params>
    std::optional<t_ret> operator()(t_time p_timeout,
                                    t_function &p_function,
                                    t_params &&... p_params) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        t_ret _ret;

        std::thread _th([&]() -> void {
            _ret = p_function(_stop, std::forward<t_params>(p_params)...);
            TRA("ret = ", _ret);

            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            DEB("no timeout");
            TRA("ret = ", _ret);
            _th.join();
            return {std::move(_ret)};
        }

        DEB("timeout");
        *_stop = true;
        _th.join();
        return {};
    }
};

/// \brief Synchronous execution, but with timeout control
///
/// \tparam t_time type of the time used to specify timeout
///
/// \tparam t_function function to be executed. It must have this signature:
/// <tt>std::function<void(tenacitas::type::ptr<bool> p_bool, t_params...
/// p_params)></tt>. The \p p_bool is how \p execute will let the function know
/// that it has exceed the time limit for its execution. The function should
/// between its instructions to check for \p p_bool, to check if it should stop
/// executing.
///
/// \tparam t_params... type of the parameters to the function
///
/// \param p_timeout maximum amount of time that the function has to finish its
/// execution. It must be one of the types defined in <tt>std::chrono</tt>
///
/// \param p_function function to execute in \p p_timeout time, with the
/// signature described above.
///
/// \param p_param... arguments to \p p_function
///
/// \return \p true if \p p_function executed in less than \p p_timeout, or \p
/// false otherwise.

template <>
struct executer_t<void> {
    template <typename t_time, typename t_function, typename... t_params>
    bool operator()(t_time p_timeout,
                    t_function &p_function,
                    t_params &&... p_params) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop {std::make_shared<bool>(false)};

        std::thread _th([&]() -> void {
            p_function(_stop, std::forward<t_params>(p_params)...);

            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock {_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            DEB("no timeout");
            _th.join();
            return true;
        }

        DEB("timeout");
        *_stop = true;
        _th.join();
        return false;
    }
};

template <typename t_ret>
struct result_traits {
    typedef std::optional<t_ret> result;
};

template <>
struct result_traits<void> {
    typedef bool result;
};

// \brief Implements a circular queue which size is increased if it
// becomes full
//
// \tparam t_data defines the types of the data contained in the
// buffer
template <typename t_data>
struct circular_unlimited_size_queue_t {
    // \brief Constructor
    //
    // \param p_size the number of initial slots in the queue
    circular_unlimited_size_queue_t(size_t p_size = 1) {
        m_root = create_node();
        node_ptr _p = m_root;
        for (size_t _i = 0; _i < p_size; ++_i) {
            _p = insert(_p, t_data());
        }
        //    this->m_size = p_size;
        m_write = m_root;
        m_read = m_root;
    }

    ~circular_unlimited_size_queue_t() {}

    // \brief Copy constructor not allowed
    circular_unlimited_size_queue_t(const circular_unlimited_size_queue_t &) =
        delete;

    // \brief Move constructor
    circular_unlimited_size_queue_t(circular_unlimited_size_queue_t &&p_queue) {
        m_root = p_queue.m_root;
        m_write = p_queue.m_write;
        m_read = p_queue.m_read;
        m_amount = p_queue.m_amount;
    }

    // \brief Copy assignment not allowed
    circular_unlimited_size_queue_t &
    operator=(const circular_unlimited_size_queue_t &) = delete;

    // \brief Move assignment
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

    // \brief Traverses the queue
    //
    // \param p_function will be called for every data in the queue
    void traverse(std::function<void(const t_data &)> p_visitor) const {
        node_ptr _p = m_root;
        while (_p && (_p->m_next != m_root)) {
            p_visitor(_p->m_data);
            _p = _p->m_next;
        }
        p_visitor(_p->m_data);
    }

    // \brief Adds a t_data object to the queue
    //
    // \param p_data is a t_data to be added
    void add(const t_data &p_data) {
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

    // \brief Moves a t_data object to the queue
    //
    // \param p_data is a t_data to be added
    void add(t_data &&p_data) {
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

    // \brief Tries to get a t_data object from the queue
    //
    // \return {true, a valid t_data}, if it was possible to get; {false,
    // ---} if it was not possible
    std::pair<bool, t_data> get() {
        std::lock_guard<std::mutex> _lock(m_mutex);
        if (empty()) {
            return {false, t_data()};
        }

        t_data _data(m_read->m_data);
        m_read = m_read->m_next;
        --m_amount;
        return {true, std::move(_data)};
    }

    // \brief Informs if the queue is full
    inline bool full() { return m_amount == this->m_size; }

    // \brief Informs if the queue is empty
    inline bool empty() { return m_amount == 0; }

    // \brief Informs the total capacity of the queue
    inline size_t capacity() const { return this->m_size; }

    // \brief Informs the current number of slots occupied in the
    // queue
    inline size_t occupied() const { return m_amount; }

private:
    // \brief Node of the linked list used to implement the queue
    struct node {
        // \brief Type of pointer
        typedef std::shared_ptr<node> ptr;

        node() = default;

        // \brief Copy constructor not allowed
        node(const node &) = delete;

        node(node &&) = default;

        // \brief Copy assignment not allowed
        node &operator=(const node &) = delete;

        node &operator=(node &&) = default;

        // \brief Constructor
        //
        // \param p_data is a t_data to be moved into the node
        node(t_data &&p_data)
            : m_data(std::move(p_data)) {}

        // \brief Constructor
        //
        // \param p_data is a t_data to be copied into the node
        node(const t_data &p_data)
            : m_data(p_data) {}

        // \brief data in the node
        t_data m_data;

        // \brief next node
        ptr m_next;

        // \brief previous node
        ptr m_prev;
    };

    // \brief Alias for the pointer to a node
    typedef typename node::ptr node_ptr;

private:
    // \brief Inserts a node in the list after a node
    //
    // \param p_node which the new node will be inserted in front of
    //
    // \param p_data data inserted in the new node
    //
    // \return the new node
    node_ptr insert(node_ptr p_node, const t_data &p_data) {
        node_ptr _new_node = create_node(std::move(p_data));

        _new_node->m_next = p_node->m_next;
        _new_node->m_prev = p_node;

        p_node->m_next->m_prev = _new_node;
        p_node->m_next = _new_node;
        ++this->m_size;
        return _new_node;
    }

    // \brief Inserts a node in the list after a node
    //
    // \param p_node which the new node will be inserted in front of
    //
    // \param p_data data inserted in the new node
    //
    // \return the new node
    node_ptr insert(node_ptr p_node, t_data &&p_data) {
        node_ptr _new_node = create_node(std::move(std::move(p_data)));

        _new_node->m_next = p_node->m_next;
        _new_node->m_prev = p_node;

        p_node->m_next->m_prev = _new_node;
        p_node->m_next = _new_node;
        ++this->m_size;
        return _new_node;
    }

    // \brief Creates a new node
    //
    // \return the new node
    node_ptr create_node() {
        node_ptr _p(std::make_shared<node>());
        _p->m_next = _p;
        _p->m_prev = _p;
        return _p;
    }

    // \brief Creates a new node, defining its data
    //
    // \param p_data is the data inside the new node
    //
    // \return the new node
    node_ptr create_node(const t_data &p_data) {
        node_ptr _p(std::make_shared<node>(p_data));
        _p->m_next = _p;
        _p->m_prev = _p;
        return _p;
    }

    // \brief Creates a new node, defining its data
    //
    // \param p_data is the data inside the new node
    //
    // \return the new node
    node_ptr create_node(t_data &&p_data) {
        node_ptr _p(std::make_shared<node>(std::move(p_data)));
        _p->m_next = _p;
        _p->m_prev = _p;
        return _p;
    }

private:
    // \brief The first node of the queue
    node_ptr m_root;

    // \brief The node where the next write, i.e., new data
    // insertion, should be done
    node_ptr m_write;

    // \brief The node where the next read, i.e., new data
    // extraction, should be done
    node_ptr m_read;

    // \brief Amount of nodes actually used
    size_t m_amount {0};

    // \brief Amount of nodes in the queue
    size_t m_size {0};

    // \brief Controls insertion
    std::mutex m_mutex;
};

// \brief A group of functions that compete to handle an evnt added to a
// queue
//
// \param t_data type of data to be processed
template <typename t_event>
class handling_t {
public:
    // \brief Type of function that will handle the data
    typedef handler_t<t_event> handler;

public:
    // \brief Default constructor not allowed
    handling_t() = delete;

    // \brief Constructor
    //
    // \tparam t_time is the type of time used to define the timeout for all
    // the handler functions. It must be one of the defined in std::chrono,
    // like std::chrono::seconds
    //
    // \param p_timeout is the value of timeout for all the handler
    // functions
    //
    // \param p_priority is the priority of this handling among other
    // handlings of t_event
    template <typename t_time>
    handling_t(t_time p_timeout, priority p_priority = 125)
        : m_timeout(calendar::convert<timeout>(p_timeout))
        , m_priority(p_priority)
        , m_queue(1) {}

    // \brief Copy constructor not allowed
    handling_t(const handling_t &) = delete;

    // \brief Move constructor not allowed
    handling_t(handling_t &&) = delete;

    // \brief Copy assignemnt not allowed
    handling_t &operator=(const handling_t &) = delete;

    // \brief Move assignemnt not allowed
    handling_t &operator=(handling_t &&) = delete;

    // \brief Destructor
    ~handling_t() {
        DEB(m_id, "entering");
        stop();
        DEB(m_id, "leaving");
    }

    // \brief Adds a event to be handled
    //
    // \param p_event is the event to be handled
    //
    // \return \p true if it was added, \p false otherwise
    bool add_data(const t_event &p_event) {
        DEB(m_id, "adding ", p_event);
        m_queue.add(p_event);
        m_cond.notify_all();
        {
            std::lock_guard<std::mutex> _lock(m_mutex);
            ++m_queued_data;
        }
        return true;
    }

    // \return the amount of events added
    inline size_t amount_added() const { return m_queue.capacity(); }

    // \brief Adds one handler
    //
    // \param p_handler the \p handler to be added
    void add_handler(handler &&p_handler) {
        std::lock_guard<std::mutex> _lock(m_add_handler);

        m_loops.push_back(std::thread(&handling_t<t_event>::handler_loop, this,
                                      std::move(p_handler)));
        m_stopped = false;
    }

    // \brief Adds a bunch of handler functions
    //
    // \param p_num_workers defines the number of handlers to be added
    //
    // \param p_factory is a function that creates handlers
    void add_handler(uint16_t p_num_workers,
                     std::function<handler()> p_factory) {
        for (uint16_t _i = 0; _i < p_num_workers; ++_i) {
            add_handler(p_factory());
        }
    }

    // \return Informs if the handling is stopped
    inline bool is_stopped() const { return m_stopped; }

    // \return The capacity if the queue
    inline size_t capacity() const { return m_queue.capacity(); }

    // \return The amount of slots occupied in the \p queue
    inline size_t occupied() const { return m_queue.occupied(); }

    // \return The timeout defined for this handling,
    template <typename t_time>
    inline t_time get_timeout() const {
        return calendar::convert<t_time>(m_timeout);
    }

    // \return The identifier of this handling
    inline const handling_id &get_id() const { return m_id; }

    // \return The tenacitas::async::priority of this handling
    inline const priority &get_priority() const { return m_priority; }

    // \brief Sets the tenacitas::async::priority of this handling
    inline void set_priority(priority p_priority) { m_priority = p_priority; }

    // \return Returns the size of the queue of \p t_event
    size_t get_size() const { return m_queue.capacity(); }

    // \return Returns the amount of \p t_event objects in the queue
    size_t get_occupied() const { return m_queue.occupied(); }

    // \brief Less-than
    // \p handling is ordered by tenacitas::async::priority
    inline bool operator<(const handling_t &p_handlers) const {
        return m_priority < p_handlers.m_priority;
    }

    // \brief Greater-than
    // \p handling is ordered by tenacitas::async::priority
    inline bool operator>(const handling_t &p_handlers) const {
        return m_priority > p_handlers.m_priority;
    }

    // \brief Not-equal
    // \p handling is compared by tenacitas::async::handling_id
    inline bool operator!=(const handling_t &p_handlers) const {
        return m_id != p_handlers.m_id;
    }

    // \brief Equal-to
    // \p handling is compared by tenacitas::async::handling_id
    inline bool operator==(const handling_t &p_handlers) const {
        return m_id == p_handlers.m_id;
    }

private:
    // \brief Type of group of loops
    typedef typename std::vector<std::thread> loops;

    // \brief Type of the queue used to store the event to be handled
    typedef circular_unlimited_size_queue_t<t_event> queue;

private:
    // \brief loop that gets events from the queue and passes to a handler
    //
    // \param p_handler is a \p handler which will handle the events
    void handler_loop(handler &&p_handler) {
        number::id _loop_id;
        DEB(m_id, ':', _loop_id,
            "entering loop, m_timeout = ", this->m_timeout.count());

        while (true) {
            {
                std::unique_lock<std::mutex> _lock(m_mutex);
                m_cond.wait(_lock, [this, _loop_id]() -> bool {
                    if (m_stopped) {
                        DEB(m_id, ':', _loop_id, "stopped");
                        return true;
                    }
                    if (!m_queue.empty()) {
                        DEB(m_id, ':', _loop_id, "there is data");
                        return true;
                    }
                    DEB(m_id, ':', _loop_id, "waiting");
                    return false;
                });
            }

            if (m_stopped) {
                DEB(m_id, ':', _loop_id, "stop");
                break;
            }

            std::pair<bool, t_event> _maybe_data {m_queue.get()};

            if (this->m_stopped) {
                DEB(m_id, ':', _loop_id, "stop");
                break;
            }

            if (_maybe_data.first) {
                t_event _data {std::move(_maybe_data.second)};

                DEB(m_id, ':', _loop_id, "data = ", _data);

                if (this->m_stopped) {
                    DEB(m_id, ':', _loop_id, "stop");
                    break;
                }

                if (!execute(m_timeout, p_handler, std::move(_data))) {
                    WAR(m_id, ':', _loop_id, "timeout ", _data);
                }
            } else {
                DEB(m_id, ':', _loop_id, "no data available");
            }
        }
    }

    // \brief Empties the queue of the events not handled yet
    //
    // TODO test it
    inline void empty_queue() {
        while (!m_queue.empty()) {
            m_cond.notify_all();
        }
    }

    // \brief Stops this handling
    inline void stop() {
        if (m_stopped) {
            DEB(m_id, "not stopping because it is stopped");
            return;
        }

        DEB(m_id, "stoping");

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
    // \brief Maximum amount of time a handler has to complete
    timeout m_timeout;

    // \brief Priority of this handling
    priority m_priority;

    // \brief Queue where \p t_event objectg will be inserted for the
    // handlers to compete for handling
    queue m_queue;

    // \brief Identifier of this handling
    handling_id m_id;

    // \brief Asynchronous loops, where the handlers are running
    loops m_loops;

    // \brief Indicates if the worker is running
    std::atomic<bool> m_stopped {true};

    // \brief Amount of queued data
    size_t m_queued_data {0};

    // \brief Controls access to the \p m_loops while inserting a
    // new \p handler
    std::mutex m_add_handler;

    // \brief Controls access to inserting data
    std::mutex m_mutex;

    // \brief Controls access to the data produced
    std::condition_variable m_cond;
};

// \brief Compile time event identifier generator
template <typename t_event>
struct event_id_t {
    static number::id value;
};

template <typename t_event>
number::id event_id_t<t_event>::value;

// \brief Dispatches an event to a list of handlings
// Each handling handles the same event in a different  way, has its own
// definition of timeout for the handlers, its own definition of priority
//
// \tparam is the type of event this dispatcher distributes
template <typename t_event>
struct dispatcher_t {
    // \brief Type of handler
    typedef handler_t<t_event> handler;

    ~dispatcher_t() = default;

    // \brief Adds a handling to the dispatcher, which will
    // handle a event in a specific way
    //
    // \tparam t_time is the type of time used to define the
    // timeout for all the handler functions. It must be one of the
    // defined in std::chrono, like std::chrono::seconds
    //
    // \param p_timeout is the value of timeout for all the handler
    // functions
    //
    // \param p_priority is the priority of this handling
    //
    // \return a \p handling_id, identinfying the handling
    template <typename t_time>
    static inline handling_id add_handling(t_time p_timeout = 5s,
                                           priority p_priority = 125) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_list.push_back(std::make_unique<handling>(p_timeout, p_priority));
        handling_id _id = m_list.back()->get_id();
        sort();
        return _id;
    }

    // \brief Adds a handling to the dispatcher, which will
    // handle a event in a specific way, and adds a \p handler to this
    // handling
    //
    // \tparam t_time is the type of time used to define the
    // timeout for all the handler functions. It must be one of the
    // defined in std::chrono, like std::chrono::seconds
    //
    // \param p_handler is the handler function to be added
    //
    // \param p_timeout is the value of timeout for all the handler
    // functions
    //
    // \param p_priority is the priority of this handling
    //
    // \return a \p handling_id, identinfying the handling
    template <typename t_time>
    static handling_id add_handling(handler &&p_handler,
                                    t_time p_timeout = 5s,
                                    priority p_priority = 125) {
        handling_id _id = add_handling(p_timeout, p_priority);
        add_handler(_id, std::move(p_handler));
        return _id;
    }

    // \brief Sets the priority for a handling
    //
    // \param p_id is the identifier of the handling
    //
    // \param p_priority is the priority to be set for the handling
    static void set_priority(const handling_id &p_id, priority p_priority) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            (*_ite)->set_priority(p_priority);
            sort();
        }
    }

    // \brief Retrieves the priority for a handling, if found
    //
    // \param p_id is the identifier of the handling
    //
    // \return the priority of the handling, if \p p_id exists
    static std::optional<priority> get_priority(const handling_id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return {(*_ite)->get_priority()};
        }
        return {};
    }

    // \brief Sends an event yo be handled
    // This event will be copied to all the handlings, and one of the
    // handler functions in each handlig will handle the event
    //
    // \param p_event is the event to be handled
    static void send(const t_event &p_event) {
        for (handling_ptr &_handling_ptr : m_list) {
            INF(get_id(), "sending ", p_event, " to pool ",
                _handling_ptr->get_id());
            _handling_ptr->add_data(p_event);
        }
    }

    // \brief Adds a handler function to a handling
    //
    // \param p_id is the identifier of the handling
    //
    // \param p_handler is the handler function to be added
    static void add_handler(const handling_id &p_handling_id,
                            handler &&p_handler) {
        auto _handling_ite = find(p_handling_id);
        auto _end = m_list.end();
        if (_handling_ite != _end) {
            DEB(get_id(), "adding another handler for ", p_handling_id);
            (*_handling_ite)->add_handler(std::move(p_handler));
        }
    }

    // \brief Adds a bunch of handler function to a handling
    //
    // \param p_id is the identifier of the handling
    //
    // \param p_num_workers defines the number of handler functions
    // to be added
    //
    // \param p_factory is a function that creates handler function
    static void add_handler(const handling_id &p_handling_id,
                            uint16_t p_num_workers,
                            std::function<handler()> p_factory) {
        iterator _handling_ite = find(p_handling_id);
        if (_handling_ite != m_list.end()) {
            (*_handling_ite)->add_handler(p_num_workers, p_factory);
        }
    }

    // \brief Traverse the groups of handlers
    //
    // \param p_visitor is a function that will be called for each
    // handling.
    static void
    traverse(std::function<void(const handling_id &,
                                priority,
                                const std::chrono::milliseconds &)> p_visitor) {
        for (const handling_ptr &_handling_ptr : m_list) {
            p_visitor(_handling_ptr->get_id(), _handling_ptr->get_priority(),
                      _handling_ptr
                          ->template get_timeout<std::chrono::milliseconds>());
        }
    }

    // \brief Retrieves the size of the queue of events for a handling
    //
    // \param p_id is the identifier of the handling
    ///
    // \return the size of the event queue
    static size_t size(const handling_id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return (*_ite)->get_size();
        }
        return 0;
    }

    // \brief Retrieves how many positions in the queue of events for a
    // handling are occupied
    //
    // \param p_id is the identifier of the handling
    ///
    // \return the number of occupied positions
    static size_t occupied(const handling_id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return (*_ite)->get_occupied();
        }
        return 0;
    }

    // \brief Waits for all the events in all the groups of  handlers to be
    // handled
    //
    // TODO test it
    static void wait() {
        DEB(event_id_t<t_event>::value, "  - starting to wait");
        for (handling_ptr &_handling_ptr : m_list) {
            _handling_ptr->empty_queue();
        }
        DEB(event_id_t<t_event>::value, "  - finished waiting");
    }

    // \brief Retrieves the id associated to \p t_event
    static number::id get_id() { return event_id_t<t_event>::value; }

private:
    // \brief Alias for a handling for this event
    typedef internal::handling_t<t_event> handling;

    // \brief Pointer to a handling
    typedef std::unique_ptr<handling> handling_ptr;

    // \brief List of handlings
    typedef std::list<handling_ptr> handling_list;

    // \brief Iterator for the list of handlers
    typedef typename handling_list::iterator iterator;

private:
    // \brief Finds a handling based on a handling_id
    //
    // \return an iterator to the handling, or m_list.end() if not
    static iterator find(const handling_id &p_id) {
        auto _cmp = [&p_id](const handling_ptr &p_handling) -> bool {
            return p_id == p_handling->get_id();
        };
        return std::find_if(m_list.begin(), m_list.end(), _cmp);
    }

    // \brief Inserts a handling to the list
    //
    // \param p_handling is the handling to be added
    static inline void insert(handling_ptr &&p_handling) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_list.push_back(std::move(p_handling));
        //    m_list.back().start();
        sort();
    }

    // \brief Sorts the list of handlings in descending priority order
    static inline void sort() {
        m_list.sort(
            [](const handling_ptr &p_i1, const handling_ptr &p_i2) -> bool {
                return (p_i1->get_priority() > p_i2->get_priority());
            });
    }

private:
    // \brief The list of handling
    static handling_list m_list;

    // \brief Access control
    static std::mutex m_mutex;
};

template <typename t_data>
typename dispatcher_t<t_data>::handling_list dispatcher_t<t_data>::m_list;

template <typename t_data>
std::mutex dispatcher_t<t_data>::m_mutex;

} // namespace internal

/// \brief Synchronous function with collaborative timeout control
///
/// \tparam t_time type of time used to define timeout. It must be one of the
/// defined in std::chrono, such as std::chrono::seconds
///
/// \tparam t_function is the type of function that will be executed
/// synchronously. It must have this signature:
///
/// \code
/// return-type operator()(std::shared_ptr<bool>, params-type...)
/// \endcode
///
/// Where \p return-type is the type returned by the function. It can \p void,
/// or any other type or class.
///
/// Where <tt>std::shared_ptr<bool></tt> is a pointer to \p bool that will
/// become \p true if the function being executed exceeds the time limit to
/// execute. It is how the \p execute function tells the function being executed
/// that a timeout occurred. So, the function being executed should, as much as
/// possible, during its execution, to check for the value of this pointer. If
/// it becomes \p true, the function must return, as its execution is no longer
/// necessary, neither its return, if any, is expected.
///
/// Where params-type is a variadic number of parameters, if any, as the
/// function being executed needs.
///
/// \param p_timeout is the amount of time the function being executed has to
/// finish its execution
///
/// \param p_function is the function being executed, with the signature
/// described above
///
/// \param p_params are the parameters expected by \p p_function, if any
///
/// \return If \p p_function returns, say a \p int16_t, \p execute returns
/// std::optional<int16_t>, which will contain a \p int16_t if \p p_function
/// executes in less than \p p_timeout time, i.e., no timeout occurrs; otherwise
/// no value is returned in the std::optional.
///
/// However, if \p p_function's return is \p void, then \p execute returns a \p
/// bool, which is \p true if no timeout occurrs; or \p false otherwise.
template <typename t_time, typename t_function, typename... t_params>
typename internal::result_traits<
    std::invoke_result_t<t_function, type::ptr<bool>, t_params...>>::result
execute(t_time p_timeout, t_function &p_function, t_params &&... p_params) {

    typedef internal::executer_t<
        std::invoke_result_t<t_function, type::ptr<bool>, t_params...>>
        executer;
    executer _executer;
    return _executer(p_timeout, p_function,
                     std::forward<t_params>(p_params)...);
}

/// \brief Periodically executes a function
///
/// \tparam use makes tenacitas::tester::test to be compiled only if actually
/// used
template <bool use = true>
struct sleeping_loop_t {

    /// \brief Signature of the function that will be called in each round of
    /// the loop
    ///
    /// \param p_bool is a means for the object that will call the handler
    /// function to inform that the handler may stop working, as the timeout
    /// defined for it to finish its work has expired. So, the handler function
    /// should, as most as possible, between its instructions, to check for the
    /// value o \p p_bool, in order to continue to the following instruction, or
    /// to interrupt its execution
    typedef std::function<void(ptr<bool> p_bool)> function;

    /// \brief Constructor
    ///
    /// \tparam t_timeout is the type of time used to define the timeout fot the
    /// function periodically called
    ///
    /// \tparam t_interval is the type of time used to define the amount of time
    /// that the function will be called
    template <typename t_timeout, typename t_interval>
    sleeping_loop_t(function p_function,
                    t_timeout p_timeout,
                    t_interval p_interval)
        : m_function(p_function)
        , m_timeout(calendar::convert<internal::timeout>(p_timeout))
        , m_interval(calendar::convert<internal::interval>(p_interval)) {}
    sleeping_loop_t() = delete;

    sleeping_loop_t(const sleeping_loop_t &) = delete;
    sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;

    /// \brief Destrutor
    /// The loops stops calling the function
    ///
    ~sleeping_loop_t() { stop(); }

    /// \brief Move consructor
    sleeping_loop_t(sleeping_loop_t &&p_loop) {
        bool _stopped = p_loop.m_stopped;

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

    /// \brief Move assignment
    sleeping_loop_t &operator=(sleeping_loop_t &&p_loop) {
        if (this != &p_loop) {

            bool _stopped = p_loop.m_stopped;

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

    /// \brief Starts calling the function periodically
    void start() {
        if (!m_stopped) {
            return;
        }
        m_stopped = false;

        DEB("starting loop thread");
        m_thread = std::thread([this]() { loop(); });
    }

    /// \brief Stops the loop, if it was started
    void stop() {

        if (m_stopped) {
            DEB("not stopping because it is stopped");
            return;
        }
        DEB("stopping");
        m_stopped = true;
        m_cond.notify_one();

        if (m_thread.get_id() == std::thread::id()) {
            DEB("not joining because m_thread.get_id() == "
                "std::thread::id()");
            return;
        }
        DEB("m_thread.get_id() != std::thread::id()");

        if (!m_thread.joinable()) {
            DEB("not joining because m_thread is not joinable");
            return;
        }
        DEB("m_thread.joinable()");

        {
            std::lock_guard<std::mutex> _lock(m_mutex_join);
            m_thread.join();
        }
        DEB("leaving stop");
    }

    /// \brief Retrieves if the loop was stopped
    inline bool is_stopped() const { return m_stopped; }

private:
    /// \brief Loop where the function will be called
    void loop() {
        DEB("entering loop, m_timeout = ", m_timeout.count());

        while (true) {
            if (m_stopped) {
                DEB("stop");
                break;
            }

            DEB("calling handler");

            if (!execute(m_timeout, m_function)) {
                WAR("timeout");
            }

            if (m_stopped) {
                DEB("stop");
                break;
            }

            DEB("waiting for ", m_interval.count(),
                "ms to elaps, or a stop order");
            {
                std::unique_lock<std::mutex> _lock(m_mutex_interval);
                if ((m_cond.wait_for(_lock, m_interval) ==
                     std::cv_status::no_timeout) &&
                    (m_stopped)) {
                    DEB("ordered to stop");
                    break;
                }
            }
            DEB("", m_interval.count(), "ms elapsed");
        }
        DEB("leaving loop");
    }

private:
    /// \brief Function that will be called in each round of the loop
    function m_function;

    /// \brief Maximum time that \p m_function will have to execute
    internal::timeout m_timeout;

    /// \brief Interval for calling m_function
    internal::interval m_interval;

    /// \brief Indicates that the loop must stop
    std::atomic<bool> m_stopped {true};

    /// \brief Thread where the \p loop method will run
    std::thread m_thread;

    /// \brief Protects joining the thread
    std::mutex m_mutex_join;

    /// \brief Controls the interval execution
    std::mutex m_mutex_interval;

    /// \brief Controls the interval execution
    std::condition_variable m_cond;
};

// #########################################

/// \brief Adds a handling to receive events to be handled
///
/// \tparam t_event is the type of event to be added to the handling
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the event will be placed in the queue.
///
/// \return the handling id of the handling added
template <typename t_event, typename t_time>
static inline handling_id add_handling(t_time p_timeout = 5s,
                                       priority p_priority = 125) {
    return internal::dispatcher_t<t_event>::add_handling(p_timeout, p_priority);
}

/// \brief Adds a handling to receive events to be handled, and adds a handler
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_handler is the handler function to be added
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the event will be placed in the queue.
///
/// \return the handling id of the handling added
template <typename t_event, typename t_time>
static inline handling_id add_handling(handler_t<t_event> &&p_handler,
                                       t_time p_timeout = 5s,
                                       priority p_priority = 125) {
    return internal::dispatcher_t<t_event>::add_handling(std::move(p_handler),
                                                         p_timeout, p_priority);
}

/// \brief Defines the priority of a handling
///
/// The priority of the handling defines the order in which an event will be
/// handled
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_handling is the identifier of the handling
///
/// \param p_priority is the priority of the handling. This defines the order
/// that the event will be placed in the handling.
template <typename t_event>
static inline void set_priority(const handling_id &p_handling,
                                priority p_priority) {
    internal::dispatcher_t<t_event>::set_priority(p_handling, p_priority);
}

/// \brief Retrieves the priority of a handling
/// The priority of the handling defines the order in which a event will be
/// added.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_handling is the identifier of the queue
///
/// \return The priority of the queue, if a handling with \p handling_id was
/// found
template <typename t_event>
static inline std::optional<priority>
get_priority(const handling_id &p_handling) {
    return internal::dispatcher_t<t_event>::get_priority(p_handling);
}

/// \brief Dispatches an event to all handlings associated to the event
///
/// \tparam t_event is the type of event to be added to the handling
///
/// \param p_event is the event to be copied to all handlings
template <typename t_event>
static inline void dispatch(const t_event &p_event) {
    internal::dispatcher_t<t_event>::send(p_event);
}

/// \brief Dispatches an event to all handlings associated to the event
///
/// \tparam t_event is the type of event to be added to the handling
///
/// \param p_event is the event to be copied to all handlings
template <typename t_event>
static inline void dispatch(t_event &&p_event) {
    internal::dispatcher_t<t_event>::send(p_event);
}

/// \brief Adds a handler to a handling
///
/// It is possible to add many handlers to the same handling. Those handlers
/// will compete with each other to handle a added event.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_handling is the identifier of the handling to which this handler
/// will be added to
///
/// \param p_handler is the function that will handle an event added to the
/// handling
template <typename t_event>
static inline void add_handler(const handling_id &p_handling,
                               handler_t<t_event> &&p_handler) {
    internal::dispatcher_t<t_event>::add_handler(p_handling,
                                                 std::move(p_handler));
}

/// \brief Adds a bunch of handlers to a handling
///
/// It is possible to add many handlers to the same handling. Those handlers
/// will compete with each other to handle a added event.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \param p_handling_id is the identifier of the handling to which this handler
/// will be added to
///
/// \param p_num_workers defines the number of handler functions
/// to be added
///
/// \param p_factory is a function that creates handler function
template <typename t_event>
static inline void add_handler(const handling_id &p_handling,
                               uint16_t p_num_workers,
                               std::function<handler_t<t_event>()> p_factory) {
    internal::dispatcher_t<t_event>::add_handler(p_handling, p_num_workers,
                                                 p_factory);
}

/// \brief Adds a handling to receive events to be handled, and adds a handler
/// to it.
///
/// It is possible to add many handlers to the same handling. Those handlers
/// will compete with each other to handle a added event.
///
/// \tparam t_event is the type of event to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_handling_id is the identifier of the handling to which this handler
/// will be added to
///
/// \param p_handler is the function that will handle an event added to the
/// handling
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the event will be placed in the queue.
///
/// \return the handling id of the handling added
template <typename t_event, typename t_time>
static inline handling_id add_handler(handler_t<t_event> &&p_handler,
                                      t_time p_timeout,
                                      priority p_priority = 125) {
    number::id _id = add_handling<t_event>(p_timeout, p_priority);
    internal::dispatcher_t<t_event>::add_handler(_id, std::move(p_handler));
    return _id;
}

} // namespace async
} // namespace tenacitas

#endif
