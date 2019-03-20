QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.async_000

CONFIG+=test

SOURCES += \
    ../../../../../code/concurrent/test/async_000/main.cpp

include (../../../common.pri)

LIBS+=-ltenacitas.logger
