#ifndef TNCT_NUMERATE_CPT_IS_ENUM_H
#define TNCT_NUMERATE_CPT_IS_ENUM_H

#include "tnct/ostream/cpt/has_output_operator.h"

namespace tnct::numerate::cpt
{

template <typename t>
concept is_enum =
    std::is_enum_v<t> && tnct::ostream::cpt::has_output_operator<t>;

}

#endif
