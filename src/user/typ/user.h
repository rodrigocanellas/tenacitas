#ifndef TENACITAS_LIB_USER_TYP_USER_H
#define TENACITAS_LIB_USER_TYP_USER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com


#include <tenacitas.lib/src/user/typ/id.h>
#include <tenacitas.lib/src/user/typ/name.h>
#include <tenacitas.lib/src/user/typ/email.h>

namespace tenacitas::lib::user::typ {

struct user
{
    lib::user::typ::id id;
    lib::user::typ::name name;
    lib::user::typ::email email;
};

}

#endif
