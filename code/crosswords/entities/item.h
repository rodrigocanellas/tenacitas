#ifndef TENACITAS_CROSSWORDS_ENTITIES_ITEM_H
#define TENACITAS_CROSSWORDS_ENTITIES_ITEM_H

#include <string>
#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace crosswords {
namespace entities {

struct item {

    enum class difficulty : uint8_t {
        hard = 4,
        medium = 3,
        easy = 2,
        fill = 1
    };

    friend std::ostream& operator<<(std::ostream& p_out
                                    , const item & p_item) {

        p_out << "["
              << p_item.m_word << ","
              << p_item.diff2str()
              << "]";

        return p_out;
    }


    inline item(std::string && p_value, difficulty p_difficulty)
        : m_word(std::move(p_value))
        , m_difficulty(p_difficulty) {}

    item()=delete;
    item(const item&)=delete;
    item(item&&)noexcept=default;
    item&operator=(const item&)=delete;
    item&operator=(item&&)noexcept=default;
    ~item()=default;

    inline const std::string& get_value()const{return m_word;}
    inline difficulty get_difficulty()const{return m_difficulty;}

    inline bool operator==(const item & p_item) const {
        return m_word == p_item.m_word;
    }

    inline bool operator!=(const item & p_item) const {
        return m_word != p_item.m_word;
    }

    inline bool operator<(const item & p_item) const {
        return m_word <  p_item.m_word;
    }

    inline std::string::size_type get_size() const {return m_word.size();}

private:
    std::string diff2str()const {
        switch(m_difficulty) {
            case difficulty::easy:
                return "easy";
            case difficulty::medium:
                return "medium";
            case difficulty::hard:
                return "hard";
            default:
                return "fill";
        }
    }


private:
    std::string m_word;
    difficulty m_difficulty;
};

} // namespace entities
} // namespace crosswords
} // namespace tenacitas

#endif // WORD_H
