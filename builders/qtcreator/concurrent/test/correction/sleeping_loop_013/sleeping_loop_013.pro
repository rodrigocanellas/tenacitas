QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.sleeping_loop_013

CONFIG+=test

HEADERS +=

SOURCES += \
    ../../../../../code/concurrent/test/sleeping_loop_013/main.cpp

include (../../../common.pri)

LIBS+=-ltenacitas.logger
LIBS += -ltenacitas.tester.business
