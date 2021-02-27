#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include <tenacitas/calendar.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>
#include <tenacitas/number.h>
#include <tenacitas/program.h>

using namespace tenacitas;

struct log_formater {
  void operator()(int argc, char **argv) {
    if (argc < 5) {
      std::cout << "Syntax: " << argv[0]
                << " --in <log-file-to-format> --out <log-file-formated>"
                << std::endl;
      return;
    }
    try {
      if (!define_all_max(argc, argv)) {
        return;
      }

      m_in.close();
      m_in.open(m_name_in);
      if (m_in.bad()) {
        ERR(logger::log, "error reseting position in input file");
        return;
      }

      output();

    } catch (std::exception &_ex) {
      FAT(logger::log, _ex.what());
    }
  }

private:
  typedef std::function<void(const std::string &p_line,
                             std::string::size_type p_1,
                             std::string::size_type p_2)>
      parse_field;

private:
  void output() {
    std::string _line;
    if (find_first_separator(_line) == std::string::npos) {
      return;
    }

    auto _log_level = [this](const std::string &p_line,
                             std::string::size_type p_1,
                             std::string::size_type p_2) -> void {
      m_out.write(p_line.c_str() + p_1, p_2 - p_1);
      m_out << m_separator;
      m_out.flush();
    };

    auto _timestamp = [this](const std::string &p_line,
                             std::string::size_type p_1,
                             std::string::size_type p_2) -> void {
      std::string _timestamp_str{&p_line[p_1], &p_line[p_2]};
      auto _timestamp_num = std::stoull(_timestamp_str);
      m_out << calendar::now<>::microsecs_str(_timestamp_num) << m_separator;
      m_out.flush();
    };

    auto _function_name = [this](const std::string &p_line,
                                 std::string::size_type p_1,
                                 std::string::size_type p_2) -> void {
      const auto _last_dir_sep = find_last_dir_separator(p_line, p_1, p_2);
      const std::string _file_name{&p_line[_last_dir_sep], &p_line[p_2]};

      //      m_out.write(_file_name.c_str() + p_1, _file_name.size());

      m_out << _file_name;
      const uint8_t _size = static_cast<uint8_t>(_file_name.size());
      if (_size < m_max_file_name) {
        m_out << std::string(m_max_file_name - _size, ' ');
      }
      m_out << m_separator;
      m_out.flush();
    };

    auto _thread_id = [this](const std::string &p_line,
                             std::string::size_type p_1,
                             std::string::size_type p_2) -> void {
      const uint8_t _size = static_cast<uint8_t>(p_2 - p_1);
      if (_size >= m_thread_id_size) {
        m_out << p_line.substr(p_2 - m_thread_id_size, m_thread_id_size);
      } else {
        m_out << std::string(m_thread_id_size - _size, '0');
        m_out.write(p_line.c_str() + p_1, p_2 - p_1);
      }
      m_out << m_separator;
      m_out.flush();
    };

    auto _line_number = [this](const std::string &p_line,
                               std::string::size_type p_1,
                               std::string::size_type p_2) -> void {
      auto _num = std::stoul(std::string(&p_line[p_1], &p_line[p_2]));
      m_out << number::format(_num, m_max_line_number) << m_separator;
    };

    auto _msg = [this](const std::string &p_line, std::string::size_type p_1,
                       std::string::size_type p_2) -> void {
      m_out.write(p_line.c_str() + p_1, p_2 - p_1);
      m_out << std::endl;
    };

    while (true) {

      if (!parse_line(_line, _log_level, _timestamp, _function_name,
                      _line_number, _thread_id, _msg)) {
        return;
      }

      if (!next_line(_line)) {
        return;
      }
      if (m_in.eof()) {
        break;
      }
    }
    return;
  }

  bool define_all_max(int argc, char **argv) {
    if (!open_files(argc, argv)) {
      return false;
    }

    std::string _line;
    if (find_first_separator(_line) == std::string::npos) {
      return false;
    }

    auto _do_nothing = [](const std::string &, std::string::size_type,
                          std::string::size_type) -> void {};

    auto _max_file_name = [this](const std::string &p_file_name,
                                 std::string::size_type p_1,
                                 std::string::size_type p_2) -> void {
      auto _i = find_last_dir_separator(p_file_name, p_1, p_2);

      uint8_t _size = static_cast<uint8_t>(p_2 - _i);
      if (_size > m_max_file_name) {
        m_max_file_name = _size;
      }
    };

    //    auto _max_function_name = [this](const std::string &,
    //                                     std::string::size_type p_1,
    //                                     std::string::size_type p_2) -> void {
    //      uint8_t _size = p_2 - p_1;
    //      if (_size > m_max_function_name) {
    //        m_max_function_name = _size;
    //      }
    //    };

    //    auto _max_thread_id = [this](const std::string &,
    //                                 std::string::size_type p_1,
    //                                 std::string::size_type p_2) -> void {
    //      uint8_t _size = p_2 - p_1;
    //      if (_size > m_max_thread_id) {
    //        m_max_thread_id = _size;
    //      }
    //    };

    auto _max_line_number = [this](const std::string &,
                                   std::string::size_type p_1,
                                   std::string::size_type p_2) -> void {
      uint8_t _size = p_2 - p_1;
      if (_size > m_max_line_number) {
        m_max_line_number = _size;
      }
    };

    while (true) {

      if (!parse_line(_line, _do_nothing, _do_nothing, _max_file_name,
                      _max_line_number, _do_nothing, _do_nothing)) {
        return false;
      }

      if (!next_line(_line)) {
        return false;
      }
      if (m_in.eof()) {
        break;
      }
    }
    return true;
  }

  uint8_t find_last_dir_separator(const std::string &p_file_name,
                                  std::string::size_type p_1,
                                  std::string::size_type p_2) {
    auto _i = p_2;
    for (; _i != p_1; --_i) {
      if ((p_file_name[_i] == '/') || (p_file_name[_i] == '\\')) {
        break;
      }
    }
    return (_i == p_1 ? _i : ++_i);
  }

  bool next_line(std::string &p_line) {
    std::getline(m_in, p_line);
    if (m_in.eof()) {
      DEB(logger::log, "eof");
      return true;
    }
    if (!m_in) {
      ERR(logger::log, "error reading input file");
      return false;
    }
    return true;
  }

  bool parse_line(const std::string &p_line, parse_field p_level,
                  parse_field p_timestamp, parse_field p_file_name,
                  parse_field p_line_number, parse_field p_thread_id,
                  parse_field p_msg) {

    using namespace std;
    string::size_type _p1{0};
    string::size_type _p2 = p_line.find(m_separator, _p1);
    if (_p2 == string::npos) {
      if (p_line[0] == '#') {
        // last line
        return true;
      }
      ERR(logger::log, "log level separator not found");
      return false;
    }
    p_level(p_line, _p1, _p2);

    _p1 = ++_p2;
    _p2 = p_line.find(m_separator, _p1);
    if (_p2 == string::npos) {
      ERR(logger::log, "timestamp separator not found");
      return false;
    }
    p_timestamp(p_line, _p1, _p2);

    _p1 = ++_p2;
    _p2 = p_line.find(m_separator, _p1);
    if (_p2 == string::npos) {
      ERR(logger::log, "file name separator not found");
      return false;
    }
    p_file_name(p_line, _p1, _p2);

    _p1 = ++_p2;
    _p2 = p_line.find(m_separator, _p1);
    if (_p2 == string::npos) {
      ERR(logger::log, "line number separator not found");
      return false;
    }
    p_line_number(p_line, _p1, _p2);

    _p1 = ++_p2;
    _p2 = p_line.find(m_separator, _p1);
    if (_p2 == string::npos) {
      ERR(logger::log, "thread id separator not found");
      return false;
    }
    p_thread_id(p_line, _p1, _p2);

    _p1 = ++_p2;
    p_msg(p_line, _p1, p_line.size());

    return true;
  }

  std::string::size_type find_first_separator(std::string &p_line) {
    bool _pipe_found{false};
    std::string::size_type _pos{0};
    while (true) {
      _pos = 0;
      if (m_in.eof()) {
        INF(logger::log, "end of input file");
        break;
      }
      std::getline(m_in, p_line);
      if (m_in.bad()) {
        ERR(logger::log, "error reading input file");
        break;
      }

      _pos = p_line.find('|');
      if (_pos != std::string::npos) {
        DEB(logger::log, "pipe found");
        _pipe_found = true;
        break;
      }
    }
    if (!_pipe_found) {
      ERR(logger::log, "first pipe not found");
      return std::string::npos;
    }

    return _pos;
  }

  bool open_files(int argc, char **argv) {
    program::options<> _options;
    _options.parse(argc, argv, {"in", "out"});

    if (!open_in(_options)) {
      return false;
    }

    if (!open_out(_options)) {
      return false;
    }

    return true;
  }

  bool open_in(program::options<> &p_options) {
    std::pair<bool, std::string> _p = p_options.get_single_param("in");
    if (!_p.first) {
      ERR(logger::log, "error in parameter 'in'");
      return false;
    }
    m_name_in = std::move(_p.second);
    m_in.open(m_name_in);
    if (m_in.bad()) {
      ERR(logger::log, "error opening file '", _p.second, "'");
      return false;
    }
    return true;
  }

  bool open_out(program::options<> &p_options) {
    std::pair<bool, std::string> _p = p_options.get_single_param("out");
    if (!_p.first) {
      ERR(logger::log, "error in parameter 'out'");
      return false;
    }
    m_out.open(_p.second);
    if (m_out.bad()) {
      ERR(logger::log, "error opening file '", _p.second, "'");
      return false;
    }
    return true;
  }

private:
  std::ifstream m_in;
  std::ofstream m_out;
  std::string m_name_in;

  uint8_t m_max_file_name{0};
  //  uint8_t m_max_function_name{0};

  uint8_t m_max_line_number{0};

  const uint8_t m_this_size{4};
  const uint8_t m_thread_id_size{4};

  const char m_separator = {'|'};
};

int main(int argc, char **argv) {

  log_formater _log_formater;
  _log_formater(argc, argv);

  return 0;
}
