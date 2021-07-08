QT -= core
TEMPLATE = app
TARGET = tenacitas.remote.exp.server_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/remote/server_000/main.cpp
