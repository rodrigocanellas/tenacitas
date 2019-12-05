QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = capemisa.sql.test.print

CONFIG+=test



LIBS += -lcapemisa.sql.entities
#LIBS += -lcapemisa.sql.entities.a



SOURCES += \
  ../../../../../../code/sql/test/print/main.cpp

include (../../../../common.pri)

HEADERS += \
  ../../../../../../code/sql/test/create_hosts.h


