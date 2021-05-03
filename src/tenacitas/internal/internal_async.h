#ifndef TENACITAS_INTERNAL_ASYNC_H
#define TENACITAS_INTERNAL_ASYNC_H

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

namespace internal {

using namespace tenacitas::type;

/// \brief Priority
struct priority {

    priority() {}

    priority(const priority &p_priority) : m_value(p_priority.m_value) {}

    priority(priority &&p_priority) : m_value(p_priority.m_value) {}

    ~priority() = default;

    friend std::ostream &operator<<(std::ostream &p_out, priority p_priority) {
        p_out << static_cast<uint16_t>(p_priority.m_value);
        return p_out;
    }

    bool operator<(priority p_priority) const {
        return m_value < p_priority.m_value;
    }

    bool operator>(priority p_priority) const {
        return m_value > p_priority.m_value;
    }

    bool operator==(priority p_priority) const {
        return m_value == p_priority.m_value;
    }

    bool operator!=(priority p_priority) const {
        return m_value != p_priority.m_value;
    }

    priority &operator=(const priority &p_priority) {
        m_value = p_priority.m_value;
        return *this;
    }
    priority &operator=(priority &&p_priority) {
        m_value = p_priority.m_value;
        return *this;
    }

    static const priority lowest;

    static const priority very_low;

    static const priority low;

    static const priority middle;

    static const priority high;

    static const priority very_high;

    static const priority highest;

  private:
    explicit priority(uint8_t p_value) : m_value(p_value) {}

  private:
    uint8_t m_value{1};
};

const priority priority::lowest{1};
const priority priority::very_low{45};
const priority priority::low{90};
const priority priority::middle{135};
const priority priority::high{180};
const priority priority::very_high{225};
const priority priority::highest{255};

typedef std::function<bool()> breaker;

/// \brief Type of time used to define timeout
typedef std::chrono::milliseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::milliseconds interval;

using namespace std::chrono_literals;
template <typename t_type> struct executer_traits_t {
    /// \brief returned by the executed function
    typedef t_type type;

    /// \brief result of the time controlled execution of the function
    typedef std::optional<type> result;
};

template <> struct executer_traits_t<void> {
    /// \brief returned by the executed function
    typedef void type;

    /// \brief result of the time controlled execution of the function
    typedef bool result;
};

template <typename t_ret> struct executer_t {
    template <typename t_time, typename t_function, typename... t_params>
    std::optional<t_ret> operator()(t_time p_timeout, t_function p_function,
                                    std::tuple<t_params...> &&p_tuple) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop{std::make_shared<bool>(false)};

        std::tuple<ptr<bool>, t_params...> _tuple =
            std::tuple_cat(std::make_tuple(_stop), std::move(p_tuple));

        t_ret _ret;

        std::thread _th([&p_function, &_cond, &_ret, &_tuple]() -> void {
            _ret = std::apply(p_function, std::move(_tuple));
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock{_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return {_ret};
        }

        *_stop = true;
        _th.detach();
        return {};
    }

    template <typename t_time, typename t_function, typename t_param>
    std::optional<t_ret> operator()(t_time p_timeout, t_function p_function,
                                    t_param &&p_param) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop{std::make_shared<bool>(false)};

        t_ret _ret;

        std::thread _th(
            [&p_function, _stop, &_cond, &_ret, &p_param]() -> void {
                _ret = p_function(_stop, std::move(p_param));
                _cond.notify_one();
            });

        std::unique_lock<std::mutex> _lock{_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return {_ret};
        }

        *_stop = true;
        _th.detach();
        return {};
    }

    template <typename t_time, typename t_function>
    std::optional<t_ret> operator()(t_time p_timeout, t_function p_function) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop{std::make_shared<bool>(false)};

        t_ret _ret;

        std::thread _th([&p_function, _stop, &_cond, &_ret]() -> void {
            _ret = p_function(_stop);
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock{_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return {_ret};
        }

        *_stop = true;
        _th.detach();
        return {};
    }
};

template <> struct executer_t<void> {

    template <typename t_time, typename t_function, typename... t_params>
    bool operator()(t_time p_timeout, t_function p_function,
                    std::tuple<t_params...> &&p_tuple) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop{std::make_shared<bool>(false)};

        std::tuple<ptr<bool>, t_params...> _tuple =
            std::tuple_cat(std::make_tuple(_stop), std::move(p_tuple));

        std::thread _th([&p_function, &_cond, &_tuple]() -> void {
            std::apply(p_function, std::move(_tuple));
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock{_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return true;
        }

        *_stop = true;
        _th.detach();
        return false;
    }

    template <typename t_time, typename t_function, typename t_param>
    bool operator()(t_time p_timeout, t_function p_function,
                    t_param &&p_param) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop{std::make_shared<bool>(false)};

        DEB(m_log, "param = ", p_param);

        std::thread _th([&p_function, _stop, &_cond, &p_param]() -> void {
            p_function(_stop, std::move(p_param));
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock{_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return true;
        }

        *_stop = true;
        _th.detach();
        return false;
    }

    template <typename t_time, typename t_function>
    bool operator()(t_time p_timeout, t_function p_function) {
        std::mutex _mutex;
        std::condition_variable _cond;

        ptr<bool> _stop{std::make_shared<bool>(false)};

        std::thread _th([this, &p_function, _stop, &_cond]() -> void {
            DEB(m_log, "about to execute");
            p_function(_stop);
            DEB(m_log, "execution returned");
            _cond.notify_one();
        });

        std::unique_lock<std::mutex> _lock{_mutex};
        if (_cond.wait_for(_lock, p_timeout) != std::cv_status::timeout) {
            _th.join();
            return true;
        }

        *_stop = true;
        _th.detach();
        return false;
    }

  private:
    logger::cerr<> m_log{"executer"};
};

template <typename t_time, typename t_function, typename... t_params>
typename executer_traits_t<
    std::invoke_result_t<t_function, ptr<bool>, t_params...>>::result
execute(t_time p_timeout, t_function p_function,
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

#if 0
/// \brief
enum class queue_type : uint8_t {
  /// \brief
  CIRCULAR_FIXED_SIZE = 0,
  /// \brief
  CIRCULAR_UNLIMITED_SIZE = 1
};
#endif

/// \brief
template <typename t_data> struct queue_t {

    virtual ~queue_t() {}

    queue_t(const queue_t &) = default;
    queue_t(queue_t &&) = default;
    queue_t &operator=(const queue_t &) = default;
    queue_t &operator=(queue_t &&) = default;

    virtual void add(const t_data &) = 0;
    virtual void add(t_data &&p_data) = 0;
    virtual std::optional<t_data> get() = 0;
    virtual void traverse(std ::function<void(const t_data &)>) const = 0;
    virtual bool full() const = 0;
    virtual bool empty() const = 0;
    virtual size_t capacity() const = 0;
    virtual size_t occupied() const = 0;

  protected:
    queue_t(size_t p_size = 0) : m_size(p_size) {}

  protected:
    size_t m_size{0};
};

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
/// \tparam t_data defines the types of the data contained in the buffer
template <typename t_data>
struct circular_unlimited_size_queue_t : public internal::queue_t<t_data> {

    /// \brief Constructor
    ///
    /// \param p_size the number of initial slots in the queue
    circular_unlimited_size_queue_t(size_t p_size = 10)
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
        DEB(m_log, "adding ", p_data);
        std::lock_guard<std::mutex> _lock(m_mutex);
        if (!full()) {
            m_write->m_data = p_data;
            m_write = m_write->m_next;
        } else {

            insert(m_write->m_prev, p_data);
        }
        ++m_amount;
        DEB(m_log, "capacity = ", capacity(), ", occupied = ", occupied());
    }

    /// \brief Moves a t_data object to the queue
    ///
    /// \param p_data is a t_data to be added
    void add(t_data &&p_data) override {
        DEB(m_log, "adding ", p_data);
        std::lock_guard<std::mutex> _lock(m_mutex);
        if (!full()) {
            m_write->m_data = p_data;
            m_write = m_write->m_next;
        } else {

            insert(m_write->m_prev, std::move(p_data));
        }
        ++m_amount;
        DEB(m_log, "capacity = ", capacity(), ", occupied = ", occupied());
    }

    /// \brief Gets the first t_data obect added to the queue, if there is one
    ///
    /// \return a t_data object, if there was any to be retrieved
    std::optional<t_data> get() override {
        std::lock_guard<std::mutex> _lock(m_mutex);
        if (empty()) {

            return {};
        }

        t_data _data(m_read->m_data);
        m_read = m_read->m_next;
        --m_amount;
        return {_data};
    }

    /// \brief Informs if the queue is full
    inline bool full() const override { return m_amount == this->m_size; }

    /// \brief Informs if the queue is empty
    inline bool empty() const override { return m_amount == 0; }

    /// \brief Informs the total capacity of the queue
    inline size_t capacity() const override { return this->m_size; }

    /// \brief Informs the current number of slots occupied in the queue
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
        node(t_data &&p_data) : m_data(std::move(p_data)) {}

        /// \brief Constructor
        ///
        /// \param p_data is a t_data to be copied into the node
        node(const t_data &p_data) : m_data(p_data) {}

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
    /// \param p_node which the new node will be inserted in front of
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
    /// \param p_node which the new node will be inserted in front of
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

    /// \brief The node where the next write, i.e., new data insertion, should
    /// be done
    node_ptr m_write;

    /// \brief The node where the next read, i.e., new data extraction, should
    /// be done
    node_ptr m_read;

    /// \brief Amount of nodes actually used
    size_t m_amount{0};

    /// \brief Controls insertion
    std::mutex m_mutex;

    logger::cerr<> m_log{"queue"};
};

#if 0
/// \brief
template <typename t_data, typename t_size>
std::unique_ptr<internal::queue_t<t_data>>
queue_factory(queue_type p_queue_type, t_size p_size) {
  std::unique_ptr<internal::queue_t<t_data>> _res;
  switch (p_queue_type) {
  case queue_type::CIRCULAR_FIXED_SIZE: {
    typedef circular_fixed_size_queue_t<t_data> queue;
    _res = std::unique_ptr<internal::queue_t<t_data>>(new queue(p_size));
  } break;
  case queue_type::CIRCULAR_UNLIMITED_SIZE: {
    typedef circular_unlimited_size_queue_t<t_data> queue;
    _res = std::unique_ptr<internal::queue_t<t_data>>(new queue(p_size));
  } break;
  }
  return _res;
}
#endif

/// \brief A group of functions that compete to handle a data added to a queue
///
/// \param t_data type of data to be processed
template <typename t_data> class handlers_t {
  public:
    /// \brief Type of function that will handle the data
    typedef std::function<void(ptr<bool>, t_data &&)> handler;

  public:
    /// \brief Default constructor not allowed
    handlers_t() = delete;

    /// \brief Constructor
    /// Using this constructor, this group of handlers will have the
    /// tenacitas::async::priority::lowest priority
    ///
    /// \tparam t_time is the type of time used to define the timeout for all
    /// the handler functions. It must be one of the defined in std::chrono,
    /// like std::chrono::seconds
    ///
    /// \param p_id identifier of this group of handlers
    ///
    /// \param p_timeout is the value of timeout for all the handler functions
    ///
    template <typename t_time>
    handlers_t(const number::id &p_owner, t_time p_timeout,
               const priority &p_priority = priority::middle)
        : m_owner(p_owner), m_timeout(calendar::convert<timeout>(p_timeout)),
          m_priority(p_priority), m_queue(10) {}

    /// \brief Copy constructor not allowed
    handlers_t(const handlers_t &) = delete;

    handlers_t(handlers_t &&) = delete;

    /// \brief Copy assignemnt not allowed
    handlers_t &operator=(const handlers_t &) = delete;

    handlers_t &operator=(handlers_t &&) = delete;

    ~handlers_t() {
        DEB(m_log, m_owner, ':', m_id, " - entering");
        stop();
        DEB(m_log, m_owner, ':', m_id, " - leaving");
    }

    /// \brief Adds data to be passed to a handler
    ///
    /// \param p_data is the data to be passed to a worker
    ///
    /// \return \p true if it was added, \p false otherwise
    bool add_data(const t_data &p_data) {
        DEB(m_log, m_owner, ':', m_id, " - adding ", p_data);
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
    /// \tparam t_time type of time used to define timeout for the worker
    ///
    /// \param p_function the \p worker to be added
    void add_handler(handler &&p_handler) {

        std::lock_guard<std::mutex> _lock(m_add_handler);

        m_loops.push_back(std::thread(&handlers_t<t_data>::handler_loop, this,
                                      std::move(p_handler)));
        m_stopped = false;
    }

    /// \brief Adds a bunch of handler function to a group of handlers
    ///
    /// \param p_num_workers defines the number of handler functions to be added
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
    template <typename t_time> inline t_time get_timeout() const {
        return calendar::convert<t_time>(m_timeout);
    }

    /// \return The tenacitas::async::id of this group of handlers
    inline const number::id &get_owner() const { return m_owner; }

    /// \return The tenacitas::async::id of this group of handlers
    inline const number::id &get_id() const { return m_id; }

    /// \return The tenacitas::async::priority of this group of handlers
    inline const priority &get_priority() const { return m_priority; }

    /// \brief Sets the tenacitas::async::priority of this group of handlers
    inline void set_priority(const priority &p_priority) {
        m_priority = p_priority;
    }

    /// \return Returns the size of the queue of \p t_data
    size_t get_size() const { return m_queue.capacity(); }

    /// \return Returns the amount of \p t_data objects in the queue
    size_t get_occupied() const { return m_queue.occupied(); }

    /// \brief Less-than
    /// \p handlers are ordered by tenacitas::async::priority
    inline bool operator<(const handlers_t &p_handlers) const {
        return m_priority < p_handlers.m_priority;
    }

    /// \brief Greater-than
    /// \p handlers are ordered by tenacitas::async::priority
    inline bool operator>(const handlers_t &p_handlers) const {
        return m_priority > p_handlers.m_priority;
    }

    /// \brief Not-equal
    /// \p handlers are compared by tenacitas::async::id
    inline bool operator!=(const handlers_t &p_handlers) const {
        return m_id != p_handlers.m_id;
    }

    /// \brief Equal-to
    /// \p handlers are compared by tenacitas::async::id
    inline bool operator==(const handlers_t &p_handlers) const {
        return m_id == p_handlers.m_id;
    }

    /// \return Returns if the queue o \p t_data is empty
    inline void empty_queue() {
        DEB(this->m_log, this->m_owner, ':', this->m_id, " - empty queue");
        while (!m_queue.empty()) {
            m_cond.notify_all();
        }
    }

  private:
    /// \brief Type of group of loops
    typedef typename std::vector<std::thread> loops;

    /// \brief Type of the queue used to store the data to be handled
    typedef circular_unlimited_size_queue_t<t_data> queue;

  private:
    void handler_loop(handler &&p_handler) {

        number::id _loop_id;
        DEB(m_log, m_owner, ':', m_id, ':', _loop_id,
            " - entering loop, m_timeout = ", this->m_timeout.count());

        while (true) {

            {
                std::unique_lock<std::mutex> _lock(m_mutex);
                m_cond.wait(_lock, [this, _loop_id]() -> bool {
                    if (m_stopped) {
                        DEB(m_log, m_owner, ':', m_id, ':', _loop_id,
                            " - stopped");
                        return true;
                    }
                    if (!m_queue.empty()) {
                        DEB(m_log, m_owner, ':', m_id, ':', _loop_id,
                            " - there is data");
                        return true;
                    }
                    DEB(m_log, m_owner, ':', m_id, ':', _loop_id, " - waiting");
                    return false;
                });
            }

            if (m_stopped) {
                DEB(m_log, m_owner, ':', m_id, ':', _loop_id, " - stop");
                break;
            }

            std::optional<t_data> _maybe_data{m_queue.get()};

            if (this->m_stopped) {
                DEB(m_log, m_owner, ':', m_id, ':', _loop_id, " - stop");
                break;
            }

            if (_maybe_data) {

                t_data _data{std::move(*_maybe_data)};

                DEB(m_log, m_owner, ':', m_id, ':', _loop_id,
                    " - data = ", _data);

                if (this->m_stopped) {
                    DEB(m_log, m_owner, ':', m_id, ':', _loop_id, " - stop");
                    break;
                }

                if (!execute(m_timeout, p_handler, std::move(_data))) {
                    WAR(m_log, m_owner, ':', m_id, ':', _loop_id, " - timeout ",
                        _data);
                }
            } else {
                DEB(m_log, m_owner, ':', m_id, ':', _loop_id,
                    " - no data available");
            }
        }
    }

    /// \brief Sets this group of handles to stop
    inline void stop() {
        if (m_stopped) {
            DEB(m_log, m_owner, ':', m_id,
                " - not stopping because it is stopped");
            return;
        }

        DEB(m_log, m_owner, ':', m_id, " - stoping");

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

    /// \brief Queue where data will be inserted for the handlers to compete for
    /// handling
    queue m_queue;

    /// \brief Identifier of this group of handlers
    number::id m_id;

    /// \brief Asynchronous loops, where the handlers are running
    loops m_loops;

    /// \brief Indicates if the worker is running
    std::atomic<bool> m_stopped{true};

    /// \brief Amount of queued data
    size_t m_queued_data{0};

    /// \brief Controls access to the \p m_loops while inserting a new \p
    /// handler
    std::mutex m_add_handler;

    /// \brief Controls access to attributes while the worker is stopping
    //  std::mutex m_mutex_stop;

    /// \brief Controls access to inserting data
    std::mutex m_mutex;

    /// \brief Controls access to the data produced
    std::condition_variable m_cond;

    /// \brief Logger
    logger::cerr<> m_log{"handlers"};
};

template <typename t_msg> struct msg_id_t { static number::id value; };

template <typename t_msg> number::id msg_id_t<t_msg>::value;

/// \brief Distributes a message to a list of group of handlers
/// Each group of handlers handles the same message in a different way, has
/// its own definition of timeout for the handlers, its own definition of
/// priority, and what should be done if a handler times out
///
/// \tparam is the type of message this messenger distributes
template <typename t_msg> struct messenger_t {

    /// \brief Type of handler
    typedef std::function<void(ptr<bool>, t_msg &&)> handler;

    ~messenger_t() = default;

    /// \brief Adds a group of handlers to the messenger, which will handle a
    /// message in a specific way
    ///
    /// \tparam t_time is the type of time used to define the timeout for all
    /// the handler functions. It must be one of the defined in std::chrono,
    /// like std::chrono::seconds
    ///
    /// \param p_id identifier of this group of handlers
    ///
    /// \param p_priority is the priority of this group of handlers
    ///
    /// \param p_timeout is the value of timeout for all the handler functions
    template <typename t_time>
    static inline number::id
    add_handlers(const t_time &p_timeout,
                 const priority &p_priority = priority::middle) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_list.push_back(std::make_unique<handlers>(msg_id_t<t_msg>::value,
                                                    p_timeout, p_priority));
        number::id _id = m_list.back()->get_id();
        //    DEB(m_log, msg_id_t<t_msg>::value)
        sort();
        return _id;
    }

    /// \brief Adds a group of handlers to the messenger, which will handle a
    /// message in a specific way, and adds a handler function to the group of
    /// handlers
    ///
    /// \tparam t_time is the type of time used to define the timeout for all
    /// the handler functions. It must be one of the defined in std::chrono,
    /// like std::chrono::seconds
    ///
    /// \param p_handler is the handler function to be added
    ///
    /// \param p_timeout is the value of timeout for all the handler functions
    ///
    /// \param p_priority is the priority of this group of handlers
    ///
    /// \return p_id identifier of this group of handlers
    template <typename t_time>
    static number::id
    add_handlers(handler &&p_handler, t_time p_timeout,
                 const priority &p_priority = priority::middle) {
        number::id _id = add_handlers(p_timeout, p_priority);
        add_handler(_id, std::move(p_handler));
        return _id;
    }

    /// \brief Sets the priority for a group of handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \param p_priority is the priority to be set for the group of handlers
    static void set_priority(const number::id &p_id,
                             const priority &p_priority) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            (*_ite)->set_priority(p_priority);
            sort();
        }
    }

    /// \brief Retrieves the priority for a group of handlers, if found
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \return the priority of the group of handlers, if \p p_id exists
    static std::optional<priority> get_priority(const number::id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return {(*_ite)->get_priority()};
        }
        return {};
    }

    /// \brief Sends a data to be handled
    /// This data will be copied to all the handler groups, and one of the
    /// handler functions in each handler group will handle the message
    ///
    /// \param p_msg is the message to be handled
    static void send(const t_msg &p_msg) {
        for (handlers_ptr &_handlers : m_list) {
            DEB(m_log, get_id(), " - sending ", p_msg, " to pool ",
                _handlers->get_id());
            _handlers->add_data(p_msg);
        }
    }

    /// \brief Adds a handler function to a group of handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \param p_handler is the handler function to be added
    static void add_handler(const number::id &p_id, handler &&p_handler) {
        auto _ite = find(p_id);
        auto _end = m_list.end();
        if (_ite != _end) {
            DEB(m_log, get_id(), " - adding another handler for ", p_id);
            (*_ite)->add_handler(std::move(p_handler));
        }
    }

    /// \brief Adds a bunch of handler function to a group of handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ///
    /// \param p_num_workers defines the number of handler functions to be added
    ///
    /// \param p_factory is a function that creates handler function
    static void add_handler(const number::id &p_id, uint16_t p_num_workers,
                            std::function<handler()> p_factory) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            (*_ite)->add_handler(p_num_workers, p_factory);
        }
    }

    /// \brief Traverse the groups of handlers
    ///
    /// \param p_visitor is a function that will be called for each group of
    /// handlers.
    static void traverse(std::function<void(const number::id &, priority,
                                            const std::chrono::milliseconds &)>
                             p_visitor) {
        for (const handlers_ptr &_handlers : m_list) {
            p_visitor(
                _handlers->get_id(), _handlers->get_priority(),
                _handlers->template get_timeout<std::chrono::milliseconds>());
        }
    }

    /// \brief Retrieves the size of the queue of messages for a group of
    /// handlers
    ///
    /// \param p_id is the identifier of the group of handlers
    ////
    /// \return the size of the message queue
    static size_t size(const number::id &p_id) {
        iterator _ite = find(p_id);
        if (_ite != m_list.end()) {
            return (*_ite)->get_size();
        }
        return 0;
    }

    /// \brief Retrieves how many positions in the queue of messages for a group
    /// of handlers are occupied
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

    /// \brief Waits for all the messages in all the groups of handlers to be
    /// handled
    static void wait() {
        DEB(m_log, msg_id_t<t_msg>::value, "  - starting to wait");
        for (handlers_ptr &_handlers : m_list) {
            _handlers->empty_queue();
        }
        DEB(m_log, msg_id_t<t_msg>::value, "  - finished waiting");
    }

    static number::id get_id() { return msg_id_t<t_msg>::value; }

  private:
    /// \brief Alias for the group of handlers for this message
    typedef internal::handlers_t<t_msg> handlers;

    typedef std::unique_ptr<handlers> handlers_ptr;

    /// \brief List of handlers
    typedef std::list<handlers_ptr> handlers_list;

    /// \brief Iterator for the list of handlers
    typedef typename handlers_list::iterator iterator;

  private:
    /// \brief Finds a group of handlers based on a tenacitas::async::id
    ///
    /// \return an iterator to the group of handlers, of m_list.end() if not
    static iterator find(const number::id &p_id) {
        auto _cmp = [&p_id](const handlers_ptr &p_handlers) -> bool {
            return p_id == p_handlers->get_id();
        };
        return std::find_if(m_list.begin(), m_list.end(), _cmp);
    }

    /// \brief Inserts a group of handlers to the list
    ///
    /// \param p_handlers is the group of handlers to be added
    static inline void insert(handlers_ptr &&p_handlers) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_list.push_back(std::move(p_handlers));
        //    m_list.back().start();
        sort();
    }

    /// \brief Sorts the list of group of handlers
    static inline void sort() {
        //    std::sort(m_list.begin(), m_list.end());
        m_list.sort(
            [](const handlers_ptr &p_i1, const handlers_ptr &p_i2) -> bool {
                return (*p_i1) < (*p_i2);
            });
    }

  private:
    /// \brief The list of group of handlers
    static handlers_list m_list;

    /// \brief Logger
    static logger::cerr<> m_log;

    /// \brief Access control
    static std::mutex m_mutex;
};

/// \brief
template <typename t_data>
typename messenger_t<t_data>::handlers_list messenger_t<t_data>::m_list;

template <typename t_data>
logger::cerr<> messenger_t<t_data>::m_log{"messenger"};

template <typename t_data> std::mutex messenger_t<t_data>::m_mutex;

} // namespace internal
} // namespace async
} // namespace tenacitas

#if 0
/// \brief Implements a circular queue with fixed size
///
/// Unlike the usual implementation, if the buffer is full, the insertion of an
/// element *does not* override the first element
///
///
///
/// \tparam t_data defines the types of the data contained in the buffer
///
///
template <typename t_data>
struct circular_fixed_size_queue_t : public internal::queue_t<t_data> {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Constructor
  ///
  /// \param p_size the number of slots in the queue
  circular_fixed_size_queue_t(size_t p_size)
      : internal::queue_t<t_data>(p_size), m_values(p_size) {}

  circular_fixed_size_queue_t() = delete;

  circular_fixed_size_queue_t(const circular_fixed_size_queue_t &) = delete;

  circular_fixed_size_queue_t(circular_fixed_size_queue_t &&) = default;

  circular_fixed_size_queue_t &
  operator=(const circular_fixed_size_queue_t &) = delete;

  circular_fixed_size_queue_t &
  operator=(circular_fixed_size_queue_t &&) = default;

  /// \brief Adds a t_data object to the queue
  void add(const t_data &p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = p_data;
      ++m_amount;
      if (m_write == this->m_size) {
        m_write = 0;
      }
    } else {
      DEB(this->m_log, this->m_owner, ':' , this->m_id, " - could not add ", p_data, ", because it is full");
    }
  }

  /// \brief Gets the first t_data obect added to the queue, if there is one
  std::optional<data> get() override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      DEB(this->m_log, this->m_owner, ':' , this->m_id, " - could not get because it is empty");
      return {};
    }
    data _data(m_values[m_read++]);
    --m_amount;
    if (m_read == this->m_size) {
      m_read = 0;
    }
    return {_data};
  }

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  void traverse(std ::function<void(const data &)> p_function) const override {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_amount) {
      if (_i == this->m_size) {
        _i = 0;
      }
      p_function(m_values[_i++]);
      ++_counter;
    }
  }

  /// \brief Informs if the queue is full
  inline bool full() const override { return m_amount == this->m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const override { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const override { return this->m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const override { return m_amount; }

private:
  /// \brief Queue implemented as a vector
  typedef std::vector<t_data> values;

  /// \brief Type of size of the queue
  typedef typename values::size_type size;

private:
  /// \brief Vector with the values
  values m_values;

  /// \brief Position from where to read
  size m_read = 0;

  /// \brief Position where to write
  size m_write = 0;

  /// \brief Amount of elements
  size m_amount = 0;

  /// \brief Controls access to positions
  std::mutex m_mutex;

  logger::cerr<> m_log{"circular_fixed_size_queue_t"};
};

#endif

#endif
