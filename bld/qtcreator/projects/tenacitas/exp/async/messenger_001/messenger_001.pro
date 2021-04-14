QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.messenger_001
CONFIG+=example
include (../../../../../common.pri)

SOURCES += \
  $$exp_src_dir/tenacitas/async/messenger_001/main.cpp
