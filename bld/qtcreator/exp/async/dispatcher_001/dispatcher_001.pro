QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.dispatcher_001
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/async/dispatcher_001/main.cpp
