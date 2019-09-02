QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.sleeping_loop_010

CONFIG+=test

HEADERS +=

SOURCES += \
    ../../../../../../code/concurrent/test/correction/sleeping_loop_010/main.cpp

include (../../../../common.pri)


