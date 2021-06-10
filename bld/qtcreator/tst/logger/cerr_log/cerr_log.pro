QT -= core
TEMPLATE = app
TARGET = tenacitas.logger.tst.cerr
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/logger/cerr_log/main.cpp
