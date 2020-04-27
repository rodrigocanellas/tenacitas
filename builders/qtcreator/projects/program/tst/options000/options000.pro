


CONFIG -= qt
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.program.test.options000

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/program/options000/main.cpp


LIBS += $$libs_dir/libtenacitas.program.$$static_lib_ext




