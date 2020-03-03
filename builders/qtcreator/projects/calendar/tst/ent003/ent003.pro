TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=test

SOURCES += \
  ../../../../../../code/calendar/tst/ent003/main.cpp


TARGET = tenacitas.calendar.tst.ent003


include (../../../../common.pri)

LIBS += -ltenacitas.tester.business
LIBS += -ltenacitas.logger.business
#LIBS += $$libs_dir/libtenacitas.calendar.ent.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.calendar.ent.a

