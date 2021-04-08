CONFIG -= qt
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.program.tst.exit

include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/program/exit/main.cpp
