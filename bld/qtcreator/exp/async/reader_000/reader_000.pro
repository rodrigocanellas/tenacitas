QT -= core
TEMPLATE = app
TARGET = tenacitas.async.exp.reader_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/async/reader_000/main.cpp
