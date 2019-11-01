QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.dispatcher_002

CONFIG+=test

SOURCES += \
    ../../../../../../../code/concurrent/test/correction/dispatcher_002/main.cpp

include (../../../../../common.pri)


LIBS += -ltenacitas.logger.business
LIBS += -ltenacitas.tester.business
