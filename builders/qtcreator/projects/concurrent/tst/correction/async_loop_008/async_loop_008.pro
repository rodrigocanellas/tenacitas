QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.test.async_loop_008

CONFIG+=test

SOURCES += \
    $$code_src_dir/concurrent/test/async_loop_008/main.cpp

include (../../../../common.pri)

LIBS += -ltenacitas.logger.business

LIBS += -ltenacitas.tester.business
