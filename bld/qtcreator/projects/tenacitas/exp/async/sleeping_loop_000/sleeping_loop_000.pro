QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.sleeping_loop_000
CONFIG+=example
include (../../../../../common.pri)

SOURCES += \
  $$exp_src_dir/tenacitas/async/sleeping_loop_000/main.cpp
