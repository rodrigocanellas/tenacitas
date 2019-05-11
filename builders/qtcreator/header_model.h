#ifndef TENACITAS_project_group_class_H
#define TENACITAS_project_group_class_H


/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace project {
/// \brief namespace of the group
namespace group {

  /// \brief class
struct class {

public:


    /// \brief default ctor not allowed
    inline class() = delete;

    /// \brief copy ctor not allowed
     class(const class &) = delete;

    /// \brief move ctor not allowed
     class(class &&) = delete;

    // \brief destructor
     ~class();

    /// \brief copy operator not allowed
     class & operator=(const class &) = delete;

    /// \brief move operator not allowed
     class & operator=(class &&) = delete;

    /// \brief dymanic array allocation not allowed
    void *operator new[] (size_t) = delete;

    /// \brief dymanic array deallocation allowed
    void operator delete[] (void *) = delete;

    /// \brief dymanic allocation not allowed
    void operator delete (void * p) = delete;

    /// \brief dymanic deallocation not allowed
    void *operator new(size_t) = delete;


};
} // namespace group
} // namespace project
} // namespace tenacitas


#endif
