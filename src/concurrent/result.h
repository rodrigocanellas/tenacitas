#ifndef TENACITAS_CONCURRENT_STATUS_H
#define TENACITAS_CONCURRENT_STATUS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <status/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

// static const status::owner project{1592686847};

///// \brief already_running indicates that the loop was already running
// static const status::result already_running{project, 1};

// static const status::result stop_by_destructor{project, 2};

///// \brief timeout is a result of a function execution that took longer than
///// defined
// static const status::result stopped_by_timeout{project, 3};

// static const status::result stopped_by_breaker{project, 4};

// static const status::result stopped_by_provider{project, 5};

// static const status::result stopped_by_worker{project, 6};

// static const status::result stopped_by_user{project, 7};

// static const status::result error_writing{project, 8};

// static const status::result error_reading{project, 9};

} // namespace concurrent
} // namespace tenacitas

#endif
