/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \file In this example a tenacitas::async::reader_t class reads a financial
/// transaction record that contains an account number, the value of the
/// transaction and the date it occurred.
/// There are two (types of) handling: the first will will sum all the values
/// for each account; the second one will identify the date with the biggest
/// amount of transactions, for each account.
/// For example, in the collection of transaction records:
/// {4, 300.00, 2020-11-04}
/// {17, 14.21, 2020-11-05}
/// {9, 132.10, 2020-11-05}
/// {4, 289.25, 2020-11-06}
/// {17, 32.80, 2020-11-07}
/// {9, 44.10, 2020-11-06}
/// {9, 83.95, 2020-11-07}
/// {4, 91.50, 2020-11-05}
/// {4, 290.50, 2020-11-09}
/// {17, 104.15, 2020-11-03}
/// The total for each account would be: {4, 971.25}; {9, 260.15}; {17, 142.16};
/// and the date with the biggest transaction for each account would be: {4,
/// 2020-11-04}; {9, 2020-11-05}; {17, 2020-11-03}

#include <algorithm>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <map>
#include <thread>
#include <tuple>
#include <vector>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/program.h>
#include <tenacitas.lib/type.h>

using namespace tenacitas;
using namespace tenacitas::type;
using namespace std::chrono_literals;

// type for the account number
typedef uint16_t account_number;

// type for the transaction date
typedef std::string transaction_date;

// type for the transaction value
typedef float transaction_value;

// a transaction
typedef std::tuple<account_number, transaction_value, transaction_date>
    transaction;

// collection of transaction
typedef std::vector<transaction> transactions;

// asynchronous reader
typedef async::reader_t<transaction> reader;

// collection of the sum of each account
typedef std::map<account_number, transaction_value> accounts_sum;

// collection of day of the biggest transaction of each account
typedef std::map<account_number, std::pair<transaction_date, transaction_value>>
    accounts_by_day;

// synchronous reader
struct sync_reader {
    sync_reader() { create_transactions(); }
    std::optional<transaction> operator()() {
        if (m_current == m_transactions.end()) {
            return {};
        }
        transaction _transaction {std::move(*m_current)};
        ++m_current;
        return _transaction;
    }

private:
    // create transactions
    void create_transactions() {
        m_transactions = {
            {4, 300.00, "2020-11-04"}, {17, 14.21, "2020-11-05"},
            {9, 132.10, "2020-11-05"}, {4, 289.25, "2020-11-06"},
            {17, 32.80, "2020-11-07"}, {9, 44.10, "2020-11-06"},
            {9, 83.95, "2020-11-07"},  {4, 91.50, "2020-11-05"},
            {4, 290.50, "2020-11-09"}, {17, 104.15, "2020-11-03"}};
        // set the reader controller to the first transaction
        m_current = m_transactions.begin();
    }

private:
    // collection o transactions
    transactions m_transactions;

    // controler of the current transaction being read
    transactions::const_iterator m_current;
};

// sum the transactions
struct sum_transactions {

    // references the sum of each account
    sum_transactions(std::shared_ptr<accounts_sum> p_accounts)
        : m_accounts(p_accounts) {}

    // handles a transaction read
    void operator()(sptr<const bool>, sptr<const reader::data_read> &&p_data) {
        account_number _account_number = std::get<0>(p_data->value);
        transaction_value _transaction_value = std::get<1>(p_data->value);
        accounts_sum::iterator _ite = m_accounts->find(_account_number);
        if (_ite == m_accounts->end()) {
            m_accounts->insert({_account_number, _transaction_value});
        } else {
            _ite->second += _transaction_value;
        }
    }

private:
    // the sum of each account
    std::shared_ptr<accounts_sum> m_accounts;
};

// calculates the day of the biggeest account for each account
struct biggest_day {

    // references the collection of day of the biggest transaction of each
    // account
    biggest_day(std::shared_ptr<accounts_by_day> p_accounts)
        : m_accounts(p_accounts) {}

    // handles a transaction read
    void operator()(sptr<const bool>, sptr<const reader::data_read> &&p_data) {
        account_number _account_number = std::get<0>(p_data->value);
        accounts_by_day::iterator _ite = m_accounts->find(_account_number);
        if (_ite == m_accounts->end()) {
            m_accounts->insert(
                {_account_number,
                 {std::get<2>(p_data->value), std::get<1>(p_data->value)}});
        } else {
            transaction_value _transaction_value = std::get<1>(p_data->value);
            if (_transaction_value > _ite->second.second) {
                _ite->second.first = std::get<2>(p_data->value);
                _ite->second.second = _transaction_value;
            }
        }
    }

private:
    // the collection of day of the biggest transaction of each account
    std::shared_ptr<accounts_by_day> m_accounts;
};

void Main() {

    // controls the end of \p Main
    std::mutex _mutex;
    std::condition_variable _cond;

    // the collection of the sum of each account
    std::shared_ptr<accounts_sum> _accounts_sum {
        std::make_shared<accounts_sum>()};

    // the collection of day of the biggest transaction of each account
    std::shared_ptr<accounts_by_day> _accounts_by_day {
        std::make_shared<accounts_by_day>()};

    // creates a handling for a transaction read that will sum the transactions
    // of each account
    async::add_handler<reader::data_read>(sum_transactions {_accounts_sum},
                                          500ms);

    // creates a handling for a transaction read that will calculate the day of
    // biggest transaction for each account
    async::add_handler<reader::data_read>(biggest_day {_accounts_by_day},
                                          500ms);

    // creates a handling for when all transactions are read that notifies the
    // condition variable
    async::add_handler<reader::all_data_read>(
        [&](sptr<const bool>, sptr<const reader::all_data_read> &&) -> void {
            _cond.notify_one();
        },
        1s);

    // the asynchronous reader
    reader _reader(sync_reader {});

    // starting the reading
    _reader.start();

    // waits for all the transactions to be read
    std::unique_lock<std::mutex> _lock(_mutex);
    _cond.wait(_lock);

    // reports the sums
    std::cout << "Sums:\n";
    for (const accounts_sum::value_type &_account_sum : *_accounts_sum) {
        std::cout << _account_sum.first << " - " << _account_sum.second << '\n';
    }

    // reports the day of the biggest transaction for each account
    std::cout << "Days:\n";
    for (const accounts_by_day::value_type &_account_by_day :
         *_accounts_by_day) {
        std::cout << _account_by_day.first << " - "
                  << _account_by_day.second.first << " - "
                  << _account_by_day.second.second << '\n';
    }

    // dispatches that the program can exit
    async::dispatch(program::exit_app {});
}

int main() { program::application(1s, Main); }
