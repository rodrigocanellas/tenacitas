
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.handlers_055
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/handlers/main_055.cpp

HEADERS += \
  $$test_src_dir/tst/tenacitas/async/event.h \
  $$test_src_dir/tst/tenacitas/async/handlers/test_base.h


