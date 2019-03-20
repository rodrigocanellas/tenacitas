QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.sleeping_loop_005

CONFIG+=test

HEADERS +=

SOURCES += \
    ../../../../../../code/concurrent/test/correction/sleeping_loop_005/main.cpp

include (../../../../common.pri)

LIBS+=-ltenacitas.logger
