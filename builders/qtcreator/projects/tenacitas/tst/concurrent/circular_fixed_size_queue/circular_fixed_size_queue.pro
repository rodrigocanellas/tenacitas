QT -= core
TEMPLATE = app
CONFIG+=test
TARGET = tenacitas.concurrent.tst.circular_fixed_size_queue
include (../../../../../common.pri)
SOURCES += \
  $$test_src_dir/concurrent/circular_fixed_size_queue/main.cpp


