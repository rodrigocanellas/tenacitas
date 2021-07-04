QT -= core
TEMPLATE = app
TARGET = tenacitas.remote.tst.server_000
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/remote/server_000/main.cpp

