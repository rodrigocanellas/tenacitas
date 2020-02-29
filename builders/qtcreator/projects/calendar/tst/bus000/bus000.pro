TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG+=test

SOURCES += \
  ../../../../../../code/calendar/tst/bus000/main.cpp



TARGET = tenacitas.calendar.tst.bus000


include (../../../../common.pri)

LIBS += -ltenacitas.tester.business
LIBS += -ltenacitas.logger.business
LIBS += $$libs_dir/libtenacitas.calendar.ent.$$static_lib_ext
