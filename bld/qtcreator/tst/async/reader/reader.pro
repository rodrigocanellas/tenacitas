QT -= core
TEMPLATE = app
TARGET = tenacitas.async.tst.reader
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/async/reader/main.cpp

