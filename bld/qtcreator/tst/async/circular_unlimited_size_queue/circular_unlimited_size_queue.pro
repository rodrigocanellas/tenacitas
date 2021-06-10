QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.circular_unlimited_size_queue
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/async/circular_unlimited_size_queue/main.cpp
