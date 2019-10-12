#ifndef TENACITAS_CROSSWORDS_ENTITIES_WORDS_H
#define TENACITAS_CROSSWORDS_ENTITIES_WORDS_H

#include <iomanip>
#include <vector>

#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/word.h>

namespace tenacitas {
namespace crosswords {
namespace entities {

typedef std::vector<word> words;

struct cmp_words
{
    bool operator()(const word& p_w1, const word& p_w2)
    {
        if (p_w1.get_size() > p_w2.get_size()) {
            return true;
        }
        if (p_w1.get_size() == p_w2.get_size()) {
            return p_w1.get_lexeme() > p_w2.get_lexeme();
        }
        return false;
    }
};

void
print(coordinate::x p_x_limit, coordinate::y p_y_limit, const words& p_words)
{
    using namespace std;

    typedef std::vector<std::string> matrix;

    string::size_type _x_size = p_x_limit.get_value<string::size_type>();
    string::size_type _y_size = p_y_limit.get_value<string::size_type>();

    matrix _m(_x_size, string(_y_size, ' '));

    words::const_iterator _end = p_words.end();
    for (words::const_iterator _ite = p_words.begin(); _ite != _end; ++_ite) {
        if (_ite->positioned()) {
            const coordinates& _coords = _ite->get_coordinates();
            const lexeme& _lexeme = _ite->get_lexeme();
            for (lexeme::size_type _i = 0; _i < _lexeme.size(); ++_i) {
                matrix::size_type _x =
                  _coords[_i].get_x().get_value<string::size_type>();
                string::size_type _y =
                  _coords[_i].get_y().get_value<std::string::size_type>();
                char _c = _lexeme[_i];
                _m[_x][_y] = _c;
            }
        }
    }

    cerr << "    ";
    for (matrix::size_type _x = 0; _x < _x_size; ++_x) {
        cerr << setw(2) << setfill('0') << _x << " ";
    }
    std::cerr << std::endl;
    for (std::string::size_type _y = 0; _y < _y_size; ++_y) {
        cerr << setw(2) << setfill('0') << _y << " ";
        for (matrix::size_type _x = 0; _x < _x_size; ++_x) {
            cerr << "  " << _m[_x][_y];
        }
        cerr << std::endl;
    }
}

// struct positions {

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

// private:
//    std::list<position>  m_list;
//};

} // namespace entities
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONS_H
