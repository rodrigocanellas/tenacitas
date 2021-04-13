QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.messenger_000
CONFIG+=example
include (../../../../../common.pri)

SOURCES += \
  $$exp_src_dir/tenacitas/async/messenger_000/main.cpp
