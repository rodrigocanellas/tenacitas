TEMPLATE = app
CONFIG -= qt

CONFIG+=test

TARGET = tenacitas.calendar.tst.bus001

include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.calendar.unix.$$static_lib_ext

SOURCES += \
  ../../../../../../code/calendar/tst/bus001/main.cpp
