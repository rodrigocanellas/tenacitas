QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.async_loop_006

CONFIG+=test

SOURCES += \
    ../../../../../../code/test/concurrent/correction/async_loop_006/main.cpp


include (../../../../common.pri)


LIBS += -ltenacitas.business.logger

LIBS += -ltenacitas.business.tester
