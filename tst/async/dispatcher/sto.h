#ifndef TENACITAS_TST_ASYNC_DISPATCHER_STO_H
#define TENACITAS_TST_ASYNC_DISPATCHER_STO_H

#include <cstdint>
#include <optional>
#include <sstream>
#include <string>

#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/number/typ/id.h>
#include <tenacitas.lib/tst/async/dispatcher/cfg.h>
#include <tenacitas.lib/tst/async/dispatcher/typ.h>

#include <sqlite3.h>

namespace sto {

/*
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "tests" (
        "test_id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        "sent_expected"	INTEGER NOT NULL,
        "sent_done"	INTEGER NOT NULL,
        "handled_expected"	INTEGER NOT NULL,
        "handled_done"	INTEGER NOT NULL,
        "total_time"	INTEGER NOT NULL
);
CREATE TABLE IF NOT EXISTS "subscribers" (
        "test_id"	INTEGER NOT NULL,
        "publishing_id"	INTEGER NOT NULL,
        "subscriber_id"	INTEGER NOT NULL,
        "amount"	INTEGER NOT NULL,
        FOREIGN KEY("test_id","publishing_id") REFERENCES "publishings",
        PRIMARY KEY("test_id","publishing_id","subscriber_id")
);
CREATE TABLE IF NOT EXISTS "publishings" (
        "test_id"	INTEGER NOT NULL,
        "publishing_id"	INTEGER NOT NULL,
        "sleep"	INTEGER NOT NULL,
        FOREIGN KEY("test_id") REFERENCES "tests"("test_id"),
        PRIMARY KEY("test_id","publishing_id")
);
CREATE TABLE IF NOT EXISTS "generators" (
        "test_id"	INTEGER NOT NULL,
        "generator_id"	INTEGER NOT NULL,
        "amount"	INTEGER NOT NULL,
        "interval"	INTEGER NOT NULL,
        FOREIGN KEY("test_id") REFERENCES "tests"("test_id"),
        PRIMARY KEY("test_id","generator_id")
);
COMMIT;
*/

struct saver {
  saver(const cfg::options &p_cfg) : m_sqlite_name(p_cfg.get_sqlite_file()) {}

  ~saver() { close_db(); }

  std::optional<typ::test_id>
  operator()(typ::summary &&p_summary, typ::generators &&p_generators,
             typ::publishings_results &&p_publishings) {
    try {
      open_db();

      start_transaction();

      save_summary(p_summary);

      save_generators(p_generators);

      save_publishings(p_publishings);

      commit_transaction();

      close_db();

    } catch (std::exception &_ex) {
      sqlite3_exec(m_db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
      TNCT_LOG_ERR(_ex.what());
      return std::optional<typ::test_id>();
    }
    return std::optional<typ::test_id>{m_test_id};
  }

private:
  void create_db() {
    std::string _sql{
        "BEGIN TRANSACTION;"
        "CREATE TABLE IF NOT EXISTS \"tests\" ("
        "\"test_id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "\"sent_expected\"	INTEGER NOT NULL,"
        "\"sent_done\"	INTEGER NOT NULL,"
        "\"handled_expected\"	INTEGER NOT NULL,"
        "\"handled_done\"	INTEGER NOT NULL,"
        "\"total_time\"	INTEGER NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS \"subscribers\" ("
        "\"test_id\"	INTEGER NOT NULL,"
        "\"publishing_id\"	INTEGER NOT NULL,"
        "\"subscriber_id\"	INTEGER NOT NULL,"
        "\"amount\"	INTEGER NOT NULL,"
        "FOREIGN KEY(\"test_id\",\"publishing_id\") REFERENCES "
        "\"publishings\","
        "PRIMARY KEY(\"test_id\",\"publishing_id\",\"subscriber_id\")"
        ");"
        "CREATE TABLE IF NOT EXISTS \"publishings\" ("
        "\"test_id\"	INTEGER NOT NULL,"
        "\"publishing_id\"	INTEGER NOT NULL,"
        "\"sleep\"	INTEGER NOT NULL,"
        "FOREIGN KEY(\"test_id\") REFERENCES \"tests\"(\"test_id\"),"
        "PRIMARY KEY(\"test_id\",\"publishing_id\")"
        ");"
        "CREATE TABLE IF NOT EXISTS \"generators\" ("
        "\"test_id\"	INTEGER NOT NULL,"
        "\"generator_id\"	INTEGER NOT NULL,"
        "\"amount\"	INTEGER NOT NULL,"
        "\"interval\"	INTEGER NOT NULL,"
        "FOREIGN KEY(\"test_id\") REFERENCES \"tests\"(\"test_id\"),"
        "PRIMARY KEY(\"test_id\",\"generator_id\")"
        ");"
        "COMMIT;"};
    TNCT_LOG_TST("trying to create database '", m_sqlite_name, '\'');
    char *_err{nullptr};
    if (sqlite3_exec(m_db, _sql.c_str(), nullptr, nullptr, &_err) ==
        SQLITE_OK) {
      if (_err == nullptr) {
        TNCT_LOG_TST('\'', _sql.c_str(), "' OK");
        get_last_test_id();
        return;
      }
    }

    if (_err != nullptr) {
      std::stringstream _str;
      _str << '\'' << _err << "' in '" << _sql.c_str() << "'";
      sqlite3_free(_err);
      throw std::runtime_error(_str.str());
    }
  }

  void open_db() {
    close_db();
    if (sqlite3_open(m_sqlite_name.c_str(), &m_db) != SQLITE_OK) {

      throw std::runtime_error("could not open db '" + m_sqlite_name + "'");
    }
    create_db();
    TNCT_LOG_TST("database '", m_sqlite_name, "' opened");
  }

  void start_transaction() {
    if (sqlite3_exec(m_db, "BEGIN TRANSACTION;", NULL, NULL, NULL) !=
        SQLITE_OK) {
      throw std::runtime_error("it was not possible to start transaction");
    }
  }

  void get_last_test_id() {
    sqlite3_stmt *_stmt;
    int rc = sqlite3_prepare_v2(m_db, "select max(test_id) from tests", -1,
                                &_stmt, NULL);
    if (rc != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(m_db));
    }

    rc = sqlite3_step(_stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
      sqlite3_finalize(_stmt);
      throw std::runtime_error(sqlite3_errmsg(m_db));
    }
    if (rc == SQLITE_DONE) {
      sqlite3_finalize(_stmt);
      throw std::runtime_error("max id not found");
    }

    m_test_id = sqlite3_column_int(_stmt, 0);
    TNCT_LOG_TST("test id = ", m_test_id);

    sqlite3_finalize(_stmt);
  }

  void save_summary(const typ::summary &p_summary) {
    std::stringstream _stream;

    _stream << "insert into tests(sent_expected, sent_done, handled_expected, "
               "handled_done, total_time) values ("
            << p_summary.total_sent_expected << ", " << p_summary.total_sent
            << ", " << p_summary.total_handled_expected << ", "
            << p_summary.total_handled << ", " << p_summary.duration.count()
            << "); ";

    char *_err{nullptr};
    if (sqlite3_exec(m_db, _stream.str().c_str(), nullptr, nullptr, &_err) ==
        SQLITE_OK) {
      if (_err == nullptr) {
        TNCT_LOG_TST('\'', _stream.str(), "' OK");
        get_last_test_id();
        return;
      }
    }

    if (_err != nullptr) {
      std::stringstream _str;
      _str << '\'' << _err << "' in '" << _stream.str() << "'";
      sqlite3_free(_err);
      throw std::runtime_error(_str.str());
    }
  }

  void save_generators(const typ::generators &p_generators) {
    for (const typ::generator &_generator : p_generators) {
      std::stringstream _stream;
      _stream << "insert into generators (test_id, generator_id, amount, "
                 "interval) values("
              << m_test_id << ", " << _generator.id << ", "
              << _generator.to_generate << ", " << _generator.interval.count()
              << "); ";
      char *_err{nullptr};
      if ((sqlite3_exec(m_db, _stream.str().c_str(), nullptr, nullptr, &_err) ==
           SQLITE_OK) &&
          (_err == nullptr)) {
        TNCT_LOG_TST('\'', _stream.str(), "' OK");
        continue;
      }

      if (_err != nullptr) {
        std::stringstream _str;
        _str << '\'' << _err << "' in '" << _stream.str() << "'";
        sqlite3_free(_err);
        throw std::runtime_error(_str.str());
      }
    }
  }

  void save_subscribers(async::typ::queue_id p_publishing_id,
                        const typ::subscribers_results &p_subscribers) {
    for (const typ::subscribers_results::value_type &_value : p_subscribers) {
      std::stringstream _stream;
      _stream << "insert into subscribers (test_id, publishing_id, "
                 "subscriber_id, "
                 "amount) values ("
              << m_test_id << ", " << p_publishing_id << ", " << _value.first
              << ", " << _value.second << "); ";
      char *_err{nullptr};
      if ((sqlite3_exec(m_db, _stream.str().c_str(), nullptr, nullptr, &_err) ==
           SQLITE_OK) &&
          (_err == nullptr)) {
        TNCT_LOG_TST('\'', _stream.str(), "' OK");
        continue;
      }

      if (_err != nullptr) {
        std::stringstream _str;
        _str << '\'' << _err << "' in '" << _stream.str() << "'";
        sqlite3_free(_err);
        throw std::runtime_error(_str.str());
      }
    }
  }

  void save_publishings(const typ::publishings_results &p_publishings) {
    for (const typ::publishings_results::value_type &_value : p_publishings) {
      async::typ::queue_id _publishing_id = _value.first;
      std::stringstream _stream;
      _stream << "insert into publishings (test_id, publishing_id, sleep) "
                 "values ("
              << m_test_id << ", " << _publishing_id << ", "
              << _value.second.sleep.count() << "); ";

      char *_err{nullptr};
      if ((sqlite3_exec(m_db, _stream.str().c_str(), nullptr, nullptr, &_err) ==
           SQLITE_OK) &&
          (_err == nullptr)) {
        TNCT_LOG_TST('\'', _stream.str(), "' OK");
        const typ::subscribers_results &_subscribers =
            _value.second.subscribers;
        save_subscribers(_publishing_id, _subscribers);
        continue;
      }

      if (_err != nullptr) {
        std::stringstream _str;
        _str << '\'' << _err << "' in '" << _stream.str() << "'";
        sqlite3_free(_err);
        throw std::runtime_error(_str.str());
      }
    }
  }

  void commit_transaction() {
    if (sqlite3_exec(m_db, "END TRANSACTION;", NULL, NULL, NULL) != SQLITE_OK) {
      throw std::runtime_error("it was not possible to commit transaction");
    }
    TNCT_LOG_TST("COMMITING test ", m_test_id);
  }

  void close_db() {
    if (m_db) {
      sqlite3_close(m_db);
      m_db = nullptr;
    }
  }

private:
  std::string m_sqlite_name;
  sqlite3 *m_db{nullptr};
  typ::test_id m_test_id{0};
}; // namespace sto

} // namespace sto

#endif // STO_H
