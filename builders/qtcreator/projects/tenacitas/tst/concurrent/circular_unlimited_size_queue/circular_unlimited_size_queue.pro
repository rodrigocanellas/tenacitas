QT -= core
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.concurrent.tst.circular_unlimited_size_queue
include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/circular_unlimited_size_queue/main.cpp


