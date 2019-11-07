//#ifndef TENACITAS_CROSSWORDS_MESSAGES_DISPATCHERS_H
//#define TENACITAS_CROSSWORDS_MESSAGES_DISPATCHERS_H

//#include <chrono>
//#include <functional>
//#include <string>

//#include <concurrent/business/traits.h>
//#include <concurrent/business/dispatcher.h>
//#include <crosswords/messages/not_positioned.h>
//#include <crosswords/messages/positioned.h>
//#include <crosswords/messages/to_position.h>
//#include <crosswords/messages/stop_positioning.h>

//using namespace tenacitas::concurrent::business;


//namespace tenacitas {
//namespace crosswords {
//namespace messages {

//template <typename t_log>
//struct dispatcher {

//    typedef t_log log;

//  template<typename t_msg>
//  static void subscribe(const std::string& p_queue,
//                        std::function<work_status(t_msg&&)> p_work,
//                        std::chrono::milliseconds p_work_timeout) {
//    dispatcher<log>::subscribe(p_queue, p_wor)
//  }

//private:



//  typedef concurrent::business::
//  dispatcher_t<messages::positioned<log>, log>
//  dispatcher_positioned;

//  typedef concurrent::business::
//  dispatcher_t<messages::not_positioned, log>
//  dispatcher_not_positioned;

//};


//} // namespace messages
//} // namespace crosswords
//} // namespace tenacitas

//#endif
