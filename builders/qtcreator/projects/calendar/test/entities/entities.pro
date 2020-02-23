TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=test

SOURCES += \
  ../../../../../../code/calendar/test/entities/main.cpp



QMAKE_CXXFLAGS += -std=c++11


TARGET = tenacitas.calendar.test.entities


include (../../../../common.pri)

LIBS += -ltenacitas.tester.business
LIBS += -ltenacitas.logger.business
LIBS += -ltenacitas.calendar.entities
