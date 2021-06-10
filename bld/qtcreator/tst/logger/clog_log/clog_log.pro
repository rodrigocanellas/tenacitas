QT -= core
TEMPLATE = app
TARGET = tenacitas.logger.tst.clog
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/logger/clog_log/main.cpp
