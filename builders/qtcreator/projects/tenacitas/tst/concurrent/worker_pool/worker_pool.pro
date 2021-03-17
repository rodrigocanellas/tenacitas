
QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.worker_pool
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/worker_pool/main.cpp

HEADERS += \
  ../../../../../../../tst/concurrent/msg.h


