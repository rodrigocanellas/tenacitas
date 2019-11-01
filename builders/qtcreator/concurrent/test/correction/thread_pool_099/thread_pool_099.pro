QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.correction.thread_pool_099
CONFIG+=test
SOURCES += \
    ../../../../../../code/test/concurrent/correction/thread_pool_099/main.cpp
include (../../../../common.pri)

LIBS += -ltenacitas.business.tester
LIBS += -ltenacitas.business.logger
