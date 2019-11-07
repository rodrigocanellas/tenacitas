#ifndef TENACITAS_CROSSWORDS_BUSINESS_LETTERS_DISTRIBUTIONS_H
#define TENACITAS_CROSSWORDS_BUSINESS_LETTERS_DISTRIBUTIONS_H

#include <algorithm>
#include <list>

#include <crosswords/entities/words.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/lexeme.h>

namespace tenacitas {
namespace crosswords {
namespace business {

typedef entities::word word;
typedef entities::words words;
typedef entities::lexeme lexeme;
typedef entities::lexeme::value_type letter;
typedef std::list<words::iterator> words_iterators;
typedef uint16_t amount;


struct letter_amount {

  friend std::ostream & operator<<(std::ostream & p_out, const letter_amount & p_la) {
    p_out << "(" << p_la.get_letter() << "," << p_la.get_amount() << ")";
    return p_out;
  }

  letter_amount()=delete;
  inline letter_amount(letter p_letter, amount p_amount = 1)
    : m_letter(p_letter),
      m_amount(p_amount) {}
  letter_amount(const letter_amount&)=default;
  letter_amount(letter_amount&&)noexcept=default;
  letter_amount&operator=(const letter_amount&)=default;
  letter_amount&operator=(letter_amount&&)noexcept=default;
  ~letter_amount()=default;

  inline letter get_letter()const {return m_letter;}
  inline amount get_amount()const {return m_amount;}
  inline letter_amount & operator++() {++m_amount;return *this;}
  inline void add(amount p_amount) {m_amount+=p_amount;}

  inline bool operator==(const letter_amount& p_la) const {
    return ((m_letter==p_la.m_letter) && (m_amount==p_la.m_amount));
  }

  inline bool operator!=(const letter_amount& p_la) const {
    return ((m_letter!=p_la.m_letter) || (m_amount!=p_la.m_amount));
  }

  inline bool less_than_by_letter(const letter_amount & p_la) const {
    return m_letter < p_la.m_letter;
  }

  inline bool less_than_by_amount(const letter_amount & p_la) const {
    return m_amount < p_la.m_amount;
  }

private:
  letter m_letter;
  amount m_amount;
};

struct word_letters_amount {

  typedef std::list<letter_amount> letters_amounts;

  friend std::ostream & operator<<(std::ostream & p_out, const word_letters_amount & p_wla) {
    p_out << "[" << *(p_wla.get_word_iterator()) << ": ";
    letters_amounts::const_iterator _end_letters = p_wla.end_letters();
    for (letters_amounts::const_iterator _ite = p_wla.begin_letters(); _ite != _end_letters;++_ite){
      p_out << *_ite;
      if (std::next(_ite) != _end_letters) {
        p_out << " " ;
      }
    }
    p_out << "]";
    return p_out;
  }


  explicit word_letters_amount(words::iterator p_word_iterator)
    : m_word_iterator(p_word_iterator) {
    const lexeme & _lexeme = p_word_iterator->get_lexeme();
    for (letter _letter : _lexeme) {
      letters_amounts::iterator _lai = find(_letter);
      if (_lai == m_letters_amounts.end()) {
        m_letters_amounts.push_back(letter_amount(_letter));
      }
      else {
        ++(*_lai);
      }
    }
  }

  inline words::const_iterator get_word_iterator()const{return m_word_iterator;}

  letters_amounts::const_iterator begin_letters()const{return m_letters_amounts.begin();}

  letters_amounts::const_iterator end_letters()const{return m_letters_amounts.end();}

  inline amount get_amount(letter p_letter)  {
    letters_amounts::iterator _la = find(p_letter);
    return (_la == m_letters_amounts.end() ? 0 : _la->get_amount());
  }


private:
  letters_amounts::iterator find(letter p_letter)  {
    return std::find_if(m_letters_amounts.begin(),
                        m_letters_amounts.end(),
                        [p_letter](const letter_amount & p_la) -> bool {
      return p_letter == p_la.get_letter();
    });
  }
private:
  words::iterator m_word_iterator;
  letters_amounts m_letters_amounts;
};

typedef std::list<word_letters_amount> words_letters_amounts;

words_letters_amounts create(words & p_words) {

  words_letters_amounts _wlas;
  words::iterator _end = p_words.end();
  for(words::iterator _ite = p_words.begin();_ite!=_end;++_ite) {
    _wlas.push_back(word_letters_amount(_ite));
  }
  return _wlas;

}

struct letter_distribution {

  friend std::ostream & operator<<(std::ostream & p_out, const letter_distribution & p_ld) {
    p_out << "{" << p_ld.get_letter_amount();
    words_letters_amounts_iterators::const_iterator _end = p_ld.end();
    for (words_letters_amounts_iterators::const_iterator _ite = p_ld.begin();
         _ite != _end;
         ++_ite) {
      p_out << *(*_ite) ;
      if (std::next(_ite) != _end) {
        p_out << " ";
      }
    }

    p_out << "}";
    return p_out;
  }

  typedef std::list<words_letters_amounts::iterator> words_letters_amounts_iterators;

  letter_distribution(letter p_letter, words_letters_amounts & p_words_letter_amounts)
    : m_letter_total_amount(p_letter, 0){
    words_letters_amounts::iterator _end = p_words_letter_amounts.end();
    for (words_letters_amounts::iterator _ite = p_words_letter_amounts.begin();
         _ite != _end;
         ++_ite) {
      if (_ite->get_amount(p_letter) != 0) {
        add(_ite, _ite->get_amount(p_letter));
      }
    }
  }



  inline const letter_amount & get_letter_amount()const{return m_letter_total_amount;}

  inline words_letters_amounts_iterators::const_iterator begin() const {
    return m_words_letters_amounts_iterators.begin();
  }

  inline words_letters_amounts_iterators::const_iterator end() const {
    return m_words_letters_amounts_iterators.end();
  }

private:
  void add(words_letters_amounts::iterator p_wla_ite, amount p_amount) {
    m_words_letters_amounts_iterators.push_back(p_wla_ite);
    m_letter_total_amount.add(p_amount);
  }
private:
  letter_amount m_letter_total_amount=0;
  words_letters_amounts_iterators m_words_letters_amounts_iterators;

};


struct letters_distributions {

  letters_distributions(words & p_words) {
    m_words_letters_amounts = create(p_words);
    for (letter _letter ='A'; _letter <= 'Z'; ++_letter) {
      letter_distribution _la(_letter, m_words_letters_amounts);
      if (_la.get_letter_amount().get_amount() > 0) {
        m_list.push_back(std::move(_la));
      }
    }
  }

  void sort_by_amount() {
    m_list.sort(
          [](const letter_distribution & p_left,
          const letter_distribution & p_right) -> bool {
      return p_left.get_letter_amount().get_amount() <
          p_right.get_letter_amount().get_amount();
    });
  }

  void traverse(std::function<void(const letter_distribution&)> p_function) const {
    for (const letter_distribution  & _ld : m_list) {
      p_function(_ld);
    }
  }

private:
  typedef std::list<letter_distribution>  list;
private:
  list m_list;
  words_letters_amounts m_words_letters_amounts;
};


} // namespace business
} // namespace crosswords
} // namespace tenacitas


#endif // LETTERS_DISTRIBUTIONS_H
