QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.sleeping_loop_011

CONFIG+=test

HEADERS +=

SOURCES += \
    ../../../../../../../code/concurrent/test/correction/sleeping_loop_011/main.cpp

include (../../../../../common.pri)

LIBS +=-ltenacitas.concurrent.business -ltenacitas.logger.business
LIBS += -ltenacitas.tester.business
