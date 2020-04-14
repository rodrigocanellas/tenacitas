TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.weekday_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/weekday_test/main.cpp

LIBS += $$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext
