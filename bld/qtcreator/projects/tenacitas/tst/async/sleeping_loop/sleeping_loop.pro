
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.sleeping_loop
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/sleeping_loop/main.cpp



