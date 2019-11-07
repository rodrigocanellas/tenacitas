QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.async_loop_011

CONFIG+=test

SOURCES += \
    ../../../../../../code/concurrent/tst/async_loop_011/main.cpp


include (../../../../common.pri)

LIBS += -ltenacitas.logger.business


LIBS += -ltenacitas.tester.business
