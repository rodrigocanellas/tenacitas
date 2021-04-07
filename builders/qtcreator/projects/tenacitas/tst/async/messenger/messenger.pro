QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.messenger
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/async/messenger/main.cpp

HEADERS += \
  $$test_src_dir/async/msg.h

