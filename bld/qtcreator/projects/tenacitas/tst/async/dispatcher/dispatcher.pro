QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.dispatcher
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/dispatcher/main.cpp

HEADERS += \
  $$test_src_dir/tst/tenacitas/async/event.h


