TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.hour_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/hour_test/main.cpp

LIBS += $$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext
