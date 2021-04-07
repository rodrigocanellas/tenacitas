
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.sleeping_loop
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/async/sleeping_loop/main.cpp



