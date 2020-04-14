TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar.tst.amount_default_ctor

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/calendar/amount_default_ctor/main.cpp

LIBS += $$libs_dir/libtenacitas.calendar.gregorian.$$static_lib_ext
