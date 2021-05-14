
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.handlers_118
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/handlers/main_118.cpp

HEADERS += \
  $$test_src_dir/tst/tenacitas/async/msg.h \
  $$test_src_dir/tst/tenacitas/async/handlers/test_base.h


