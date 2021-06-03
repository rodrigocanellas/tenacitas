QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.dispatcher_000
CONFIG+=example
include (../../../../../common.pri)

SOURCES += \
  $$exp_src_dir/tenacitas/async/dispatcher_000/main.cpp
