#ifndef TENACITAS_project_group_class_H
#define TENACITAS_project_group_class_H


/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com



/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace project {
/// \brief namespace of the group
namespace group {

  /// \brief class
struct class {

public:


    /** @brief @brief not allowed */
    inline class() = delete;

    /** @brief not allowed */
     class(const class &) = delete;

    /** @brief not allowed */
     class(class &&) = delete;

    /** Destructor */
     ~class();

    /** @brief not allowed */
     class & operator=(const class &) = delete;

    /** @brief not allowed */
     class & operator=(class &&) = delete;

    /** @brief not allowed */
    void *operator new[] (size_t) = delete;

    /** @brief not allowed */
    void operator delete[] (void *) = delete;

    /** @brief not allowed */
    void operator delete (void * p) = delete;

    /** @brief not allowed */
    void *operator new(size_t) = delete;


};
} // namespace group
} // namespace project
} // namespace tenacitas


#endif
