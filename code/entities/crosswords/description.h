#ifndef TENACITAS_ENTITIES_CROSSWORDS_DESCRIPTION_H
#define TENACITAS_ENTITIES_CROSSWORDS_DESCRIPTION_H

#include <string>

namespace tenacitas {
namespace entities {
namespace crosswords {

typedef std::string description;

//struct description {
//    friend std::ostream & operator<<(std::ostream & p_out
//                                     , const description & p_description) {
//        p_out << p_description.m_value;
//        return p_out;
//    }

//    inline description(const std::string & p_value)
//        : m_value(p_value) {}
//    inline description(std::string && p_value)
//        : m_value(std::move(p_value)) {}
//    inline description(const char * p_value)
//        : m_value(p_value) {}
//    description()=default;
//    description(const description&)=default;
//    description(description &&)noexcept=default;
//    description&operator=(const description&)=default;
//    description&operator=(description &&)noexcept=default;
//    ~description()=default;

//    inline operator std::string() const {return m_value;}

//private:
//    std::string m_value;
//};


} // namespace crosswords
} // namespace entities
} // namespace tenacitas


#endif // DESCRIPTION_H
