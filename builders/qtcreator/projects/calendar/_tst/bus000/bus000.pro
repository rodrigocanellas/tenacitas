TEMPLATE = app

CONFIG -= qt

CONFIG+=test

TARGET = tenacitas.calendar._tst.bus000

include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

LIBS += $$libs_dir/libtenacitas.calendar.unix.$$static_lib_ext

SOURCES += \
  ../../../../../../code/calendar/_tst/bus000/main.cpp
