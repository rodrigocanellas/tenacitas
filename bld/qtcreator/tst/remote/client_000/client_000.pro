QT -= core
TEMPLATE = app
TARGET = tenacitas.remote.tst.client_000
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/remote/client_000/main.cpp

