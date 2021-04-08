QT -= core
TEMPLATE = app
CONFIG+=test
TARGET = tenacitas.async.tst.circular_fixed_size_queue
include (../../../../../common.pri)
SOURCES += \
  $$test_src_dir/tenacitas/async/circular_fixed_size_queue/main.cpp


