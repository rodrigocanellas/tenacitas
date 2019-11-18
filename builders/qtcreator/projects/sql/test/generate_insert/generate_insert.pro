QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = capemisa.sql.test.generate_insert

CONFIG+=test


include (../../../../common.pri)

LIBS += -lcapemisa.sql.entities

SOURCES += \
    ../../../../../../code/sql/test/generate_insert/main.cpp
