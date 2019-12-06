QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = capemisa.sql.test.generators

CONFIG+=test

include (../../../../common.pri)

LIBS += -lcapemisa.sql.entities
LIBS += -lcapemisa.sql.business



SOURCES += \
  ../../../../../../code/sql/test/generators/main.cpp



