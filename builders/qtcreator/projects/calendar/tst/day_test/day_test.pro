TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.day_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/day_test/main.cpp

