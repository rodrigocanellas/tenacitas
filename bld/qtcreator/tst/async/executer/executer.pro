QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.executer
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/async/executer/main.cpp

