TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=test

SOURCES += \
  ../../../../../../code/calendar/tst/ent001/main.cpp




TARGET = tenacitas.calendar.tst.ent001


include (../../../../common.pri)

LIBS += -ltenacitas.tester.business
LIBS += $$libs_dir/libtenacitas.logger.business.a
#LIBS += $$libs_dir/libtenacitas.calendar.ent.a
#LIBS += -ltenacitas.calendar.unix
LIBS += $$libs_dir/libtenacitas.calendar.unix.a

