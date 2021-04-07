
QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.worker_pool
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/async/worker_pool/main.cpp

HEADERS += \
  ../../../../../../../tst/async/msg.h


