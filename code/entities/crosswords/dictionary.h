#ifndef TENACITAS_ENTITIES_CROSSWORDS_DICTIONARY_H
#define TENACITAS_ENTITIES_CROSSWORDS_DICTIONARY_H

#include <algorithm>
#include <list>
#include <set>

#include <entities/crosswords/item.h>

namespace tenacitas {
namespace entities {
namespace crosswords {

struct dictionary {
    typedef std::set<item> set;
    typedef set::const_iterator const_iterator;

    dictionary()=default;
    dictionary(const dictionary&)=delete;
    dictionary(dictionary&&)noexcept=default;
    dictionary&operator=(const dictionary&)=delete;
    dictionary&operator=(dictionary&&)noexcept=default;
    ~dictionary()=default;

    inline const_iterator end() const {
        return m_set.end();
    }

    inline const_iterator begin() const {
        return m_set.end();
    }

    inline const_iterator add(item && p_word) {
        return m_set.emplace(std::move(p_word)).first;
    }

    inline const_iterator find(const std::string & p_value) const {
        return std::find_if(m_set.begin()
                            , m_set.end()
                            , [&p_value](const item & p_word) -> bool {
            return (p_word.get_value() == p_value);
        });
    }

    inline std::list<const_iterator> find(uint8_t p_size
                                          , item::difficulty p_difficulty) const {
        std::list<const_iterator> _res;

        const_iterator _last = begin();
        const_iterator _end = end();

        while (true) {
            const_iterator _ite = std::find_if(_last, _end,
                                            [p_size, p_difficulty](const item & p_word) -> bool {
                return (p_word.get_difficulty() == p_difficulty) && (p_word.get_size() == p_size);
            });

            if (_ite == _end) {
                break;
            }

            _res.push_back(_ite);

        }
        return _res;
    }

private:

    set m_set;
};

} // namespace tenacitas
} // namespace entities
} // namespace tenacitas

#endif // DICTIONARY_H
