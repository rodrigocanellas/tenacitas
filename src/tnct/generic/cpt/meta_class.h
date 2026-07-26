/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_CPT_META_CLASS_H
#define TNCT_GENERIC_CPT_META_CLASS_H

namespace tnct::generic::cpt {

template <typename t_class> struct meta_class {
  using class_type = t_class;
};

} // namespace tnct::generic::cpt
#endif
