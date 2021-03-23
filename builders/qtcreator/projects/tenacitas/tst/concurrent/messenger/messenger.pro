QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.messenger
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/messenger/main.cpp

HEADERS += \
  $$test_src_dir/concurrent/msg.h

