#ifndef TENACITAS_CROSSWORDS_ENTITIES_POSITIONS_H
#define TENACITAS_CROSSWORDS_ENTITIES_POSITIONS_H

#include <list>

#include <crosswords/entities/position.h>

namespace tenacitas {
namespace crosswords {
namespace entities {

typedef std::list<position> positions;


//struct positions {

//    typedef std::list<position> list;
//    typedef list::iterator iterator;
//    typedef list::const_iterator const_iterator;
//    typedef list::size_type size;


//    void add(const std::string & p_word
//             , const std::string & p_description) {
//        m_list.push_back(
//                    position(static_cast<position::id>(m_list.size() + 1)
//                             , p_word
//                             , p_description));
//    }

//    inline const_iterator begin() const {return m_list.begin();}
//    inline const_iterator end() const {return m_list.end();}

//    inline iterator begin()  {return m_list.begin();}
//    inline iterator end()  {return m_list.end();}

//    inline size get_size() const {return m_list.size();}

//private:
//    std::list<position>  m_list;
//};

} // namespace entities
} // namespace crosswords
} // namespace tenacitas


#endif // POSITIONS_H
