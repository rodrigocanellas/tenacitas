QT -= core
TEMPLATE = app
TARGET = tenacitas.logger.tst.cout
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/logger/cout_log/main.cpp
