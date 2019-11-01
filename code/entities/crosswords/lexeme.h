#ifndef TENACITAS_ENTITIES_CROSSWORDS_LEXEME_H
#define TENACITAS_ENTITIES_CROSSWORDS_LEXEME_H

#include <string>

namespace tenacitas {
namespace entities {
namespace crosswords {

typedef std::string lexeme;

//struct word {
//    typedef std::string::size_type size;

//    friend std::ostream & operator<<(std::ostream & p_out
//                                     , const word & p_word) {
//        p_out << p_word.m_value;
//        return p_out;
//    }

//    inline word(const std::string & p_value)
//        : m_value(p_value) {}
//    inline word(std::string && p_value)
//        : m_value(std::move(p_value)) {}
//    inline word(const char * p_value)
//        : m_value(p_value) {}
//    word()=default;
//    word(const word&)=default;
//    word(word &&)noexcept=default;
//    word&operator=(const word&)=default;
//    word&operator=(word &&)noexcept=default;
//    ~word()=default;

//    inline operator std::string() const {return m_value;}

//    inline size get_size() const {return m_value.size();}

//private:
//    std::string m_value;
//};



} // namespace crosswords
} // namespace entities
} // namespace tenacitas

#endif // WORD_H
