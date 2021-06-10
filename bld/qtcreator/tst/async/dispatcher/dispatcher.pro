QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.dispatcher
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/async/dispatcher/main.cpp

HEADERS += \
  $$tst_code_dir/tst/async/event.h
