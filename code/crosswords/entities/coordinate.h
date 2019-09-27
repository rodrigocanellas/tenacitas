#ifndef TENACITAS_CROSSWORDS_ENTITIES_COORDINATE_H
#define TENACITAS_CROSSWORDS_ENTITIES_COORDINATE_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace crosswords {
namespace entities {

struct coordinate {
    struct x {

        friend std::ostream& operator<<(std::ostream& p_out, const x& p_x)
        {
            p_out << p_x.m_value;
            return p_out;
        }

        explicit x(int16_t p_value = -1)
            : m_value(p_value) {}

        x(const x &)=default;
        x(x &&)noexcept=default;
        x&operator=(const x &)=default;
        x&operator=(x &&)noexcept=default;
        ~x()=default;

        bool operator == (const x & p_x) const {
            return m_value == p_x.m_value;
        }
        bool operator != (const x & p_x) const {
            return m_value != p_x.m_value;
        }
        bool operator < (const x & p_x) const {
            return m_value < p_x.m_value;
        }
        bool operator > (const x & p_x) const {
            return m_value > p_x.m_value;
        }

        x & operator+(const x & p_x) {
            m_value += p_x.m_value;
            return *this;
        }

        template<typename t_int>
        x & operator+(t_int p_i) {
            m_value += static_cast<decltype(m_value)>(p_i);
            return *this;
        }

        x & operator+=(const x & p_x) {
            m_value += p_x.m_value;
            return *this;
        }

        template<typename t_int>
        x & operator+=(t_int p_i) {
            m_value += static_cast<decltype(m_value)>(p_i);
            return *this;
        }


        x & operator-(const x & p_x) {
            m_value -= p_x.m_value;
            return *this;
        }

        template<typename t_int>
        x & operator-(t_int p_i) {
            m_value -= static_cast<decltype(m_value)>(p_i);
            return *this;
        }


        x & operator-=(const x & p_x) {
            m_value -= p_x.m_value;
            return *this;
        }

        template<typename t_int>
        x & operator-=(t_int p_i) {
            m_value += static_cast<decltype(m_value)>(p_i);
            return *this;
        }

        template<typename t_int>
        x & operator/=(t_int p_i) {
            m_value /= static_cast<decltype(m_value)>(p_i);
            return *this;
        }

        x & operator/=(const x & p_x) {
            m_value /= p_x.m_value;
            return *this;
        }

        template<typename t_int>
        bool operator == (t_int p_i) const {
            return m_value == static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        bool operator != (t_int p_i) const {
            return m_value != static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        bool operator < (t_int p_i) const {
            return m_value < static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        bool operator > (t_int p_i) const {
            return m_value > static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        t_int get()const{return static_cast<t_int>(m_value);}
    private:
        int16_t m_value;
    };

    struct y {

        friend std::ostream& operator<<(std::ostream& p_out, const y& p_y)
        {
            p_out << p_y.m_value;
            return p_out;
        }

        explicit y(int16_t p_value = -1)
            : m_value(p_value) {}

        y(const y &)=default;
        y(y &&)noexcept=default;
        y&operator=(const y &)=default;
        y&operator=(y &&)noexcept=default;
        ~y()=default;

        bool operator == (const y & p_y) const {
            return m_value == p_y.m_value;
        }
        bool operator != (const y & p_y) const {
            return m_value != p_y.m_value;
        }
        bool operator < (const y & p_y) const {
            return m_value < p_y.m_value;
        }
        bool operator > (const y & p_y) const {
            return m_value > p_y.m_value;
        }

        y & operator+(const y & p_y) {
            m_value += p_y.m_value;
            return *this;
        }

        template<typename t_int>
        y & operator+(t_int p_i) {
            m_value += static_cast<decltype(m_value)>(p_i);
            return *this;
        }

        y & operator+=(const y & p_y) {
            m_value += p_y.m_value;
            return *this;
        }

        template<typename t_int>
        y & operator+=(t_int p_i) {
            m_value += static_cast<decltype(m_value)>(p_i);
            return *this;
        }


        y & operator-(const y & p_y) {
            m_value -= p_y.m_value;
            return *this;
        }

        template<typename t_int>
        y & operator-(t_int p_i) {
            m_value -= static_cast<decltype(m_value)>(p_i);
            return *this;
        }


        y & operator-=(const y & p_y) {
            m_value -= p_y.m_value;
            return *this;
        }

        template<typename t_int>
        y & operator-=(t_int p_i) {
            m_value += static_cast<decltype(m_value)>(p_i);
            return *this;
        }

        template<typename t_int>
        y & operator/=(t_int p_i) {
            m_value /= static_cast<decltype(m_value)>(p_i);
            return *this;
        }

        y & operator/=(const y & p_y) {
            m_value /= p_y.m_value;
            return *this;
        }

        template<typename t_int>
        bool operator == (t_int p_i) const {
            return m_value == static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        bool operator != (t_int p_i) const {
            return m_value != static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        bool operator < (t_int p_i) const {
            return m_value < static_cast<decltype(m_value)>(p_i);
        }

        template<typename t_int>
        bool operator > (t_int p_i) const {
            return m_value > static_cast<decltype(m_value)>(p_i);
        }



        template<typename t_int>
        t_int get()const{return static_cast<t_int>(m_value);}
    private:
        int16_t m_value;
    };


    friend std::ostream& operator<<(std::ostream& p_out, const coordinate& p_coord)
    {
        p_out << "(" << p_coord.m_x << "," << p_coord.m_y << ")";
        return p_out;
    }

    coordinate()=default;

    coordinate(x && p_x,y && p_y )
        : m_x(std::move(p_x))
        , m_y(std::move(p_y)) {}

    coordinate(const coordinate &)=default;
    coordinate(coordinate &&)noexcept=default;

    coordinate&operator=(const coordinate &)=default;
    coordinate&operator=(coordinate &&)noexcept=default;
    ~coordinate()=default;

    x get_x() const {return m_x;}
    y get_y() const {return m_y;}

    void reset() {
        m_x = x();
        m_y = y();
    }

    bool operator==(const coordinate &p_coord)const{
        return ((m_x == p_coord.m_x) && (m_y==p_coord.m_y));
    }

    bool operator!=(const coordinate &p_coord)const{
        return ((m_x != p_coord.m_x) && (m_y!=p_coord.m_y));
    }

    bool operator<(const coordinate &p_coord)const{
        if (m_x < p_coord.m_x) {return  true;}
        if (m_x > p_coord.m_x) {return  false;}
        return m_y < p_coord.m_y;
    }

    bool operator>(const coordinate &p_coord)const{
        if (m_x > p_coord.m_x) {return  true;}
        if (m_x < p_coord.m_x) {return  false;}
        return m_y > p_coord.m_y;
    }

    coordinate & operator+(const coordinate & p_coord) {
        m_x += p_coord.get_x();
        m_y += p_coord.get_y();
        return *this;
    }

    coordinate & operator+(const x & p_x) {
        m_x += p_x;
        return *this;
    }

    coordinate & operator-(const x & p_x) {
        m_x -= p_x;
        return *this;
    }

    coordinate & operator-(const coordinate & p_coord) {
        m_x - p_coord.get_x();
        m_y -= p_coord.get_y();
        return *this;
    }

    coordinate & operator+(const y & p_y) {
        m_y += p_y;
        return *this;
    }

    coordinate & operator-(const y & p_y) {
        m_y -= p_y;
        return *this;
    }

private:
    x m_x;
    y m_y;
};


} // namespace entities
} // namespace crosswords
} // namespace tenacitas


#endif // COORDINATE_H
