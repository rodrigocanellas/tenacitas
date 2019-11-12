#ifndef TENACITAS_CROSSWORDS_BUSINESS_LETTERS_DISTRIBUTIONS_H
#define TENACITAS_CROSSWORDS_BUSINESS_LETTERS_DISTRIBUTIONS_H

#include <algorithm>
#include <list>

#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

typedef entities::word word;
typedef entities::words words;
typedef entities::lexeme lexeme;
typedef entities::lexeme::value_type letter;
typedef std::list<words::iterator> words_iterators;
typedef uint16_t amount;

// a letter associated to an amount that it appears in some context
struct amount_of_letter
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const amount_of_letter& p_la)
  {
    p_out << "(" << p_la.get_letter() << "," << p_la.get_amount() << ")";
    return p_out;
  }

  amount_of_letter() = delete;
  inline amount_of_letter(letter p_letter, amount p_amount = 1)
    : m_letter(p_letter)
    , m_amount(p_amount)
  {}
  amount_of_letter(const amount_of_letter&) = default;
  amount_of_letter(amount_of_letter&&) noexcept = default;
  amount_of_letter& operator=(const amount_of_letter&) = default;
  amount_of_letter& operator=(amount_of_letter&&) noexcept = default;
  ~amount_of_letter() = default;

  inline letter get_letter() const { return m_letter; }
  inline amount get_amount() const { return m_amount; }
  inline void add(amount p_amount) { m_amount += p_amount; }

  inline bool operator==(const amount_of_letter& p_la) const
  {
    return ((m_letter == p_la.m_letter) && (m_amount == p_la.m_amount));
  }

  inline bool operator!=(const amount_of_letter& p_la) const
  {
    return ((m_letter != p_la.m_letter) || (m_amount != p_la.m_amount));
  }

  inline bool less_than_by_letter(const amount_of_letter& p_la) const
  {
    return m_letter < p_la.m_letter;
  }

  inline bool less_than_by_amount(const amount_of_letter& p_la) const
  {
    return m_amount < p_la.m_amount;
  }

private:
  letter m_letter;
  amount m_amount;
};

// amount of each letter in a word
struct amount_of_letters_in_word
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  amount_of_letters_in_word& /*p_wla*/)
  {
    //    p_out << "[" << *(p_wla.get_word_iterator()) << ": ";
    //    amounts_of_letters::iterator _end_letters = p_wla.end();
    //    for (amounts_of_letters::iterator _ite = p_wla.begin(); _ite !=
    //    _end_letters;
    //         ++_ite) {
    //      p_out << *_ite;
    //      if (std::next(_ite) != _end_letters) {
    //        p_out << " ";
    //      }
    //    }
    //    p_out << "]";
    return p_out;
  }

  explicit amount_of_letters_in_word(words::iterator p_word_iterator)
    : m_word_iterator(p_word_iterator)
  {
    const lexeme& _lexeme = p_word_iterator->get_lexeme();
    for (letter _letter : _lexeme) {
      amounts_of_letters::iterator _amount_of_letter_ite = find(_letter);
      if (_amount_of_letter_ite == m_amounts_of_letters.end()) {
        m_amounts_of_letters.push_back(amount_of_letter(_letter));
      } else {
        _amount_of_letter_ite->add(1);
      }
    }
  }

  inline words::iterator get_word_iterator() { return m_word_iterator; }

  void traverse_amounts_of_letters(
    std::function<void(amount_of_letter)> p_function) const
  {
    for (amount_of_letter _amount_of_letter : m_amounts_of_letters) {
      p_function(_amount_of_letter);
    }
  }

  inline amount get_amount(letter p_letter)
  {
    amounts_of_letters::iterator _la = find(p_letter);
    return (_la == m_amounts_of_letters.end() ? 0 : _la->get_amount());
  }

private:
  typedef std::list<amount_of_letter> amounts_of_letters;
  typedef amounts_of_letters::iterator amount_of_letter_iterator;

  amounts_of_letters::iterator find(letter p_letter)
  {
    return std::find_if(m_amounts_of_letters.begin(),
                        m_amounts_of_letters.end(),
                        [p_letter](const amount_of_letter& p_la) -> bool {
                          return p_letter == p_la.get_letter();
                        });
  }

private:
  words::iterator m_word_iterator;
  amounts_of_letters m_amounts_of_letters;
};

struct amount_of_letters_in_words
{
  // list of words with the amount of each letter
  typedef std::list<amount_of_letters_in_word> amount_of_letters_in_words_list;

  typedef amount_of_letters_in_words_list::iterator iterator;

  explicit amount_of_letters_in_words(words& p_words)
  {
    words::iterator _end = p_words.end();
    for (words::iterator _ite = p_words.begin(); _ite != _end; ++_ite) {
      m_amount_of_letters_in_words_list.push_back(
        amount_of_letters_in_word(_ite));
    }
  }

  void traverse(std::function<void(amount_of_letters_in_word)> p_function) const
  {
    for (amount_of_letters_in_word _amount_of_letters_in_word :
         m_amount_of_letters_in_words_list) {
      p_function(_amount_of_letters_in_word);
    }
  }

  inline iterator begin() { return m_amount_of_letters_in_words_list.begin(); }

  inline iterator end() { return m_amount_of_letters_in_words_list.end(); }

  //  void add(amount_of_letters_in_word p_wla_ite)
  //  {
  //    m_amount_of_letters_in_words_list.push_back(p_wla_ite);
  //  }

private:
private:
  amount_of_letters_in_words_list m_amount_of_letters_in_words_list;
};

// total amount of a letter in a set of words, and the words that this letter
// appears
struct letter_distribution
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  letter_distribution& /*p_ld*/)
  {
    //    p_out << "{" << p_ld.get_amount_of_letter();
    //    amount_of_letters_in_words_iterator _end = p_ld.end();
    //    for (amount_of_letters_in_words_iterator _ite = p_ld.begin(); _ite !=
    //    _end;
    //         ++_ite) {
    //      p_out << *_ite;
    //      if (std::next(_ite) != _end) {
    //        p_out << " ";
    //      }
    //    }

    //    p_out << "}";
    return p_out;
  }

  letter_distribution(letter p_letter,
                      amount_of_letters_in_words& p_amount_of_letters_in_words)
    : m_letter_total_amount(p_letter, 0)
  {
    amount_of_letters_in_words::iterator _end =
      p_amount_of_letters_in_words.end();
    for (amount_of_letters_in_words::iterator _ite =
           p_amount_of_letters_in_words.begin();
         _ite != _end;
         ++_ite) {
      if (_ite->get_amount(p_letter) != 0) {
        add(_ite, _ite->get_amount(p_letter));
      }
    }
  }

  inline const amount_of_letter& get_amount_of_letter() const
  {
    return m_letter_total_amount;
  }

  //  inline amount_of_letters_in_words_iterator begin()
  //  {
  //    return m_ite_amount_of_letters_in_words.begin();
  //  }

  //  inline amount_of_letters_in_words_iterator end()
  //  {
  //    return m_ite_amount_of_letters_in_words.end();
  //  }

  void traverse(std::function<void(amount_of_letters_in_word&)> p_function)
  {
    for (amount_of_letters_in_words::iterator _ite :
         m_amount_of_letters_in_words_iterator_list) {
      p_function(*_ite);
    }
  }

private:
  typedef std::list<amount_of_letters_in_words::iterator>
    amount_of_letters_in_words_iterator_list;

private:
  void add(amount_of_letters_in_words::iterator p_wla_ite, amount p_amount)
  {
    m_amount_of_letters_in_words_iterator_list.push_back(p_wla_ite);
    m_letter_total_amount.add(p_amount);
  }

private:
  amount_of_letter m_letter_total_amount = 0;
  amount_of_letters_in_words_iterator_list
    m_amount_of_letters_in_words_iterator_list;
};

struct letters_distributions
{

  explicit letters_distributions(words& p_words)
    : m_amount_of_letters_in_words(p_words)
  {
    for (letter _letter = 'A'; _letter <= 'Z'; ++_letter) {
      letter_distribution _letter_distribution(_letter,
                                               m_amount_of_letters_in_words);
      if (_letter_distribution.get_amount_of_letter().get_amount() > 0) {
        m_letter_distribution_list.push_back(std::move(_letter_distribution));
      }
    }

    //    m_ite_letter_distribution = m_letter_distribution_list.begin();
    //    m_end_letter_distribution = m_letter_distribution_list.end();

    //    m_ite_amount_of_letters_in_words =
    //      (*m_ite_letter_distribution->begin())->begin();
    m_end_words = p_words.end();
  }

  void sort_by_amount()
  {
    m_letter_distribution_list.sort(
      [](const letter_distribution& p_left,
         const letter_distribution& p_right) -> bool {
        return p_left.get_amount_of_letter().get_amount() <
               p_right.get_amount_of_letter().get_amount();
      });
  }

  void traverse(std::function<void(letter_distribution&)> p_function)
  {
    for (letter_distribution& _letter_distribution :
         m_letter_distribution_list) {
      p_function(_letter_distribution);
    }
  }

  words::iterator next()
  {

    //    if (m_ite_letter_distribution == m_end_letter_distribution) {
    //      return m_end_words;
    //    }

    //    letter_distribution_list::iterator _end_letter_distribution_list =
    //      m_letter_distribution_list.end();
    //    letter_distribution_list::iterator _ite_letter_distribution_list =
    //      m_letter_distribution_list.begin();

    //    for (; _ite_letter_distribution_list != _end_letter_distribution_list;
    //         ++_ite_letter_distribution_list) {
    //    }

    //    letter_distribution & _ld = *(m_letter_distribution_list.begin());

    //    letter_distribution::amount_of_letters_in_words_iterators::iterator
    //        _wlai_ite = _ld.begin();

    //    amount_of_letters_in_words::iterator _wla_ite = *_wlai_ite;

    //    amount_of_letters_in_word & _wla = *(_wla_ite);

    //    return _wla.get_word_iterator();
  }

private:
  typedef std::list<letter_distribution> letter_distribution_list;
  typedef letter_distribution_list::iterator iterator;

private:
  letter_distribution_list m_letter_distribution_list;

  amount_of_letters_in_words m_amount_of_letters_in_words;

  words::iterator m_end_words;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // LETTERS_DISTRIBUTIONS_H
