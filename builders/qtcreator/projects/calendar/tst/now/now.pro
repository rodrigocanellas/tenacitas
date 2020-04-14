TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.now

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/now/main.cpp


LIBS+=$$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext

