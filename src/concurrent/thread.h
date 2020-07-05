#ifndef TENACITAS_CONCURRENT_THREAD_H
#define TENACITAS_CONCURRENT_THREAD_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <functional>
#include <mutex>
#include <thread>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief The thread class is a wrapper for the std::thread, which joins in
/// destructor and move assignment
///
class thread {
public:
  /// \brief thread constructor
  ///
  /// \param p_func function that will run in a separated thread
  inline explicit thread(std::function<void()> &&p_func)
      : m_thread(std::move(p_func)) {}

  inline thread() = default;
  thread(const thread &) = delete;
  inline thread(thread &&) noexcept = default;
  thread &operator=(const thread &) = delete;

  ///
  /// \brief operator = move joins the \p this thread, if the thread is still
  /// running
  ///
  /// \param p_th thread to be move to \p this
  ///
  /// \return the new thread
  inline thread &operator=(thread &&p_th) noexcept {
    join();
    m_thread = std::move(p_th.m_thread);
    return *this;
  }

  /// \brief join waits for the thread to finish
  void join() {
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  /// \brief ~thread joins if the thread is still running
  inline ~thread() { join(); }

private:
  /// \brief m_thread the wrappered std::thread
  std::thread m_thread;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_THREAD_H
