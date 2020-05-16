TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.program.tst.options001

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/program/options001/main.cpp


LIBS += $$libs_dir/libtenacitas.program.$$static_lib_ext
