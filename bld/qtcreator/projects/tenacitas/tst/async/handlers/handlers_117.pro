
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.handlers_117
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/handlers/main_117.cpp

HEADERS += \
  $$test_src_dir/tst/tenacitas/async/msg.h \
  $$test_src_dir/tst/tenacitas/async/handlers/test_base.h

