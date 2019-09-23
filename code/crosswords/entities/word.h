#ifndef TENACITAS_CROSSWORDS_ENTITIES_WORD_H
#define TENACITAS_CROSSWORDS_ENTITIES_WORD_H

#include <cstdint>
#include <iostream>

#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>

namespace tenacitas {
namespace crosswords {
namespace entities {

struct word
{

    enum class direction : int8_t
    {
        unset = -1,
        vertical = 0,
        horizontal = 1,
    };

    enum class orientation : int8_t
    {
        unset = -1,
        forward = 0,
        backward = 1
    };

    typedef uint16_t id;

    typedef int16_t x;

    typedef int16_t y;

    friend std::ostream& operator<<(std::ostream& p_out, const word& p_pos)
    {
        p_out << "{\n"
              << "\t \"id\": \"" << p_pos.m_id << "\", \n"
              << "\t \"x\": \"" << p_pos.m_x << "\", \n"
              << "\t \"y\": \"" << p_pos.m_y << "\", \n"
              << "\t \"defined\": \"" << (p_pos.m_defined ? "true" : "false")
              << "\", \n"
              << "\t \"direction\": \"" << p_pos.direction2str() << "\", \n"
              << "\t \"orientation\": \"" << p_pos.orientation2str() << "\", \n"
              << "\t \"word\": \"" << p_pos.m_lexeme << "\", \n"
              << "\t \"description\": \"" << p_pos.m_description << "\", \n"
              << "\t \"answer\": \"" << p_pos.m_answer << "\"\n"
              << "}";
        return p_out;
    }

    word() = delete;

    inline word(id p_id,
                    const lexeme& p_lexeme,
                    const description& p_description)
      : m_id(p_id)
      , m_defined(false)
      , m_x(-1)
      , m_y(-1)
      , m_lexeme(p_lexeme)
      , m_description(p_description)
      , m_direction(direction::unset)
      , m_orientation(orientation::unset)
    {}

    word(const word&) = default;
    word(word&&) noexcept = default;
    word& operator=(const word&) = default;
    word& operator=(word&&) noexcept = default;
    ~word() = default;

    void position(x p_x, y p_y, direction p_direction, orientation p_orientation)
    {
        m_x = p_x;
        m_y = p_y;
        m_direction = p_direction;
        m_orientation = p_orientation;
        m_defined = true;
    }

    void unposition()
    {
        m_x = -1;
        m_y = -1;
        m_direction = direction::unset;
        m_orientation = orientation::unset;
        m_defined = false;
    }

    inline id get_id() const { return m_id; }
    inline x get_x() const { return m_x; }
    inline y get_y() const { return m_y; }
    inline const lexeme& get_lexeme() const { return m_lexeme; }
    inline direction get_direction() const { return m_direction; }
    inline orientation get_orientation() const { return m_orientation; }
    inline bool defined() const { return m_defined; }

    inline const lexeme& get_answer() const { return m_answer; }

    inline void set_answer(const std::string& p_answer) { m_answer = p_answer; }

    inline bool operator==(const word& p_pos) const
    {
        return m_id == p_pos.m_id;
    }

    inline bool operator!=(const word& p_pos) const
    {
        return m_id != p_pos.m_id;
    }

    inline bool operator<(const word& p_pos) const
    {
        return m_id < p_pos.m_id;
    }

  private:
    std::string direction2str() const
    {
        switch (m_direction) {
            case direction::vertical:
                return "vertical";
            case direction::horizontal:
                return "horizontal";
            default:
                return "not set";
        }
    }

    std::string orientation2str() const
    {
        switch (m_orientation) {
            case orientation::forward:
                return "forward";
            case orientation::backward:
                return "backward";
            default:
                return "not set";
        }
    }

  private:
    id m_id;
    bool m_defined;
    x m_x;
    y m_y;
    lexeme m_lexeme;
    description m_description;
    direction m_direction;
    orientation m_orientation;
    lexeme m_answer;
};

} // namespace entities
} // namespace crosswords
} // namespace tenacitas

#endif // POSITION_H
