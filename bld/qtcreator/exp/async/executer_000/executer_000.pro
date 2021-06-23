QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.executer_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/async/executer_000/main.cpp
