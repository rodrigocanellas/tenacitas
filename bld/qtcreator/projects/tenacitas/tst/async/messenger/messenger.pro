QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.messenger
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/async/messenger/main.cpp

HEADERS += \
  $$test_src_dir/tst/tenacitas/async/msg.h  \
  $$test_src_dir/tst/tenacitas/async/handlers/test_base.h

