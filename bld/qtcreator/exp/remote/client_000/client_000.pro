QT -= core
TEMPLATE = app
TARGET = tenacitas.remote.exp.client_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/remote/client_000/main.cpp
