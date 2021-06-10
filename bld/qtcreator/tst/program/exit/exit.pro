QT -= core
TEMPLATE = app
TARGET = tenacitas.program.tst.exit
CONFIG+=test
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$tst_code_dir/tst/program/exit/main.cpp
