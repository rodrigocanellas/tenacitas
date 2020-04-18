TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.amounts_conversions

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/amounts_conversions/main.cpp

LIBS += $$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext
