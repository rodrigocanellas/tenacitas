QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.sleeping_loop
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/async/sleeping_loop/main.cpp

