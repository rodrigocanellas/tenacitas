#ifndef TENACITAS_CROSSWORDS_ENTITIES_WORD_H
#define TENACITAS_CROSSWORDS_ENTITIES_WORD_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>

#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>

namespace tenacitas {
namespace crosswords {
namespace entities {

struct word
{
    typedef std::vector<coordinate> coordinates;

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

    friend std::ostream& operator<<(std::ostream& p_out, const word& p_pos)
    {
        p_out << "{"
              << "\"id\": \"" << p_pos.m_id << "\", "
              << "\"lexeme\": \"" << p_pos.m_lexeme << "\", "
              << "\"coord\": \"" << p_pos.m_coords[0] << "\", "
              << "\"direction\": \"" << p_pos.direction2str() << "\", "
              << "\"orientation\": \"" << p_pos.orientation2str() << "\""
              << "}";
        return p_out;
    }

    word() = delete;

    inline word(id p_id,
                const lexeme& p_lexeme,
                const description& p_description)
      : m_id(p_id)
      , m_positioned(false)
      , m_coords(p_lexeme.size(),
                 coordinate(coordinate::x(-1), coordinate::y(-1)))
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

    void position(coordinate p_coord,
                  direction p_direction,
                  orientation p_orientation)
    {
        coordinate::x _x = p_coord.get_x();
        coordinate::y _y = p_coord.get_y();
        lexeme::size_type _size = m_lexeme.size();
        if (get_direction() == direction::vertical) {
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                m_coords[_i] = { _x, _y + coordinate::y(_i) };
            }
        } else {
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                m_coords[_i] = { _x + coordinate::x(_i), _y };
            }
        }

        m_direction = p_direction;
        m_orientation = p_orientation;
        m_positioned = true;
    }

    void position(coordinate::x p_x,
                  coordinate::y p_y,
                  direction p_direction,
                  orientation p_orientation)
    {
        position(coordinate(p_x, p_y), p_direction, p_orientation);
    }

    void unposition()
    {
        reset_coords();

        m_direction = direction::unset;
        m_orientation = orientation::unset;
        m_positioned = false;
    }

    inline id get_id() const { return m_id; }
    inline coordinate::x get_x() const { return m_coords[0].get_x(); }
    inline coordinate::y get_y() const { return m_coords[0].get_y(); }
    inline const coordinates& get_coord() const { return m_coords; }
    inline const lexeme& get_lexeme() const { return m_lexeme; }
    inline direction get_direction() const { return m_direction; }
    inline orientation get_orientation() const { return m_orientation; }
    inline bool positioned() const { return m_positioned; }

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

    inline bool operator<(const word& p_pos) const { return m_id < p_pos.m_id; }

    void traverse(std::function<void(const coordinate& p_coord, char p_char)>
                    p_function) const
    {
        lexeme::size_type _size = m_lexeme.size();
        for (lexeme::size_type _i = 0; _i < _size; ++_i) {
            p_function(m_coords[_i], m_lexeme[_i]);
        }
    }

    static std::string direction2str(direction p_direction)
    {
        switch (p_direction) {
            case direction::vertical:
                return "vertical";
            case direction::horizontal:
                return "horizontal";
            default:
                return "not set";
        }
    }

    std::string orientation2str(orientation p_orientation)
    {
        switch (p_orientation) {
            case orientation::forward:
                return "forward";
            case orientation::backward:
                return "backward";
            default:
                return "not set";
        }
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

    void reset_coords()
    {
        for (coordinate& _coord : m_coords) {
            _coord.reset();
        }
    }

  private:
    id m_id;
    bool m_positioned;
    coordinates m_coords;
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
