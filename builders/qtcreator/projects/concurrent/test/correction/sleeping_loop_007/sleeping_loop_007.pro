QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.sleeping_loop_007

CONFIG+=test

HEADERS +=

SOURCES += \
    ../../../../../../../code/concurrent/test/correction/sleeping_loop_007/main.cpp

include (../../../../../common.pri)

LIBS +=-ltenacitas.concurrent.business -ltenacitas.logger.business -ltenacitas.string.business
LIBS += -ltenacitas.tester.business
