QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.async_loop_007

CONFIG+=test

SOURCES += \
    ../../../../../../../code/concurrent/test/correction/async_loop_007/main.cpp


include (../../../../../common.pri)


LIBS += -ltenacitas.logger.business



LIBS += -ltenacitas.tester.business
