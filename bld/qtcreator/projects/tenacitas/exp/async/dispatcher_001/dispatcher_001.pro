QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.dispatcher_001
CONFIG+=example
include (../../../../../common.pri)

SOURCES += \
  $$exp_src_dir/tenacitas/async/dispatcher_001/main.cpp
