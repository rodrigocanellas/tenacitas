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

static const status::owner project{1592686847};

/// \brief already_running indicates that the loop was already running
static const status::result already_running{project, 1};

/// \brief stopped_by_provider indicates that the provider function, that
/// provides data for a work function inside a loop, has no more data to
/// provide
static const status::result stopped_by_provider{project, 2};

/// \brief stopped_by_timeout indicates that the work function did not finish
/// in a spedified time
static const status::result stopped_by_timeout{project, 3};

/// \brief stopped_by_user indicates that the loop was stopped by a user
/// request
static const status::result stopped_by_user{project, 4};

/// \brief stopped_by_breaker indicates that the loop was stopped by the
/// breaker function
static const status::result stopped_by_breaker{project, 5};

} // namespace concurrent
} // namespace tenacitas

#endif
