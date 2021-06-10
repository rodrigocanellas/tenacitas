QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.handlers
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/async/handlers/main.cpp

HEADERS += \
  $$tst_code_dir/tst/async/event.h \
  $$tst_code_dir/tst/async/handlers/test_base.h
