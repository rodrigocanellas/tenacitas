TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.number.tst.uint8_align_right_fill_with_space

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/number/uint8_align_right_fill_with_space/main.cpp

LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
