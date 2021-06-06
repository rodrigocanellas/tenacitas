CONFIG -= qt
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.program.tst.exit

include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/tst/tenacitas/program/exit/main.cpp
