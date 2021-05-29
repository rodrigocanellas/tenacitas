
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.handlers_104
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/handlers/main_104.cpp

HEADERS += \
  $$test_src_dir/tst/tenacitas/async/event.h \
  $$test_src_dir/tst/tenacitas/async/handlers/test_base.h


