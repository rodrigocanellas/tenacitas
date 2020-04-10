TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.number.tst.uint32_align_left_filled_with_a

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/number/uint32_align_left_filled_with_a/main.cpp

