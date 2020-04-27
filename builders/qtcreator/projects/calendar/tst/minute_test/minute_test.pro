TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.minute_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/minute_test/main.cpp

LIBS += $$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
