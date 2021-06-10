QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.dispatcher_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/async/dispatcher_000/main.cpp
