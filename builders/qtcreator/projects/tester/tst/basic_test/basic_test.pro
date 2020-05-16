TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.tester.tst.basic_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/tester/basic_test/main.cpp

LIBS += $$libs_dir/libtenacitas.program.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
