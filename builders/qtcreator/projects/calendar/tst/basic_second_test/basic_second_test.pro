TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.basic_second_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/basic_second_test/main.cpp

LIBS += $$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext
