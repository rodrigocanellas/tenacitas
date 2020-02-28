TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=test

SOURCES += \
  ../../../../../../code/calendar/tst/ent/main.cpp



QMAKE_CXXFLAGS += -std=c++11


TARGET = tenacitas.calendar.tst.ent


include (../../../../common.pri)

LIBS += -ltenacitas.tester.business
LIBS += -ltenacitas.logger.business
LIBS += $$libs_dir/libtenacitas.calendar.ent.$$static_lib_ext
