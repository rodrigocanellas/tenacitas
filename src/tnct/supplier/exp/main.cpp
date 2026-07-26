#include <chrono>
#include <optional>

#include "tnct/format/bus/fmt.h"
#include "tnct/generic/cpt/meta_class.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/supplier/cpt/object_supplier.h"
#include "tnct/supplier/cpt/reference_supplier.h"
#include "tnct/time/cpt/chrono_convertible.h"

using tnct::format::bus::fmt;

class supplier {

public:
  using logger = tnct::log::cerr;
  using time = std::chrono::seconds;

  supplier() = default;

  supplier(const supplier &) = delete;
  supplier(supplier &&) = delete;

  supplier &operator=(const supplier &) = delete;
  supplier &operator=(supplier &&) = delete;

  void *operator new(std::size_t) = delete;

  std::optional<std::reference_wrapper<logger>>
  get(tnct::generic::cpt::meta_class<logger>) {
    return std::optional<std::reference_wrapper<logger>>{m_logger};
  }

  std::optional<time> get(tnct::generic::cpt::meta_class<time>) {
    return std::optional<time>{time{}};
  }

private:
  logger m_logger{};
};

template <typename t_supplier>

  requires(tnct::time::cpt::convertible_to_sec<typename t_supplier::time> &&
           tnct::supplier::cpt::object_supplier<t_supplier,
                                                typename t_supplier::time> &&
           tnct::log::cpt::logger<typename t_supplier::logger> &&
           tnct::supplier::cpt::reference_supplier<t_supplier,
                                                   typename t_supplier::logger>)

class xpto {

public:
  using supplier_type = t_supplier;
  using logger_type = typename supplier_type::logger;
  using time_type = typename supplier_type::time;

  xpto(supplier_type &p_supplier)
      : m_logger{p_supplier.get(tnct::generic::cpt::meta_class<logger_type>{})},
        m_time{p_supplier.get(tnct::generic::cpt::meta_class<time_type>{})} {}

  void now() {
    TNCT_LOG_INF(m_logger.value().get(),
                 fmt("m_time = ", m_time.value().count()));
  }

  void log(std::string_view p_string) {
    TNCT_LOG_INF(m_logger.value().get(), p_string);
  }

private:
  std::optional<std::reference_wrapper<logger_type>> m_logger;
  std::optional<time_type> m_time;
};

int main() {

  supplier _supplier;

  xpto _xpto{_supplier};

  _xpto.log("hello!!!");

  _xpto.now();

  return 0;
}
