TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.number.tst.int16_align_right_fill_with_zero

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/number/int16_align_right_fill_with_zero/main.cpp

