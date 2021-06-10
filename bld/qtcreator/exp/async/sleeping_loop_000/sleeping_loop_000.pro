QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.sleeping_loop_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/async/sleeping_loop_000/main.cpp
