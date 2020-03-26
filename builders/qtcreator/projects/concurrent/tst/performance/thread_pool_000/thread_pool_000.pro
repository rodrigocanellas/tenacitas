QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = app
TARGET = tenacitas.concurrent.test.performance.thread_pool_000
CONFIG+=test
SOURCES += \ 
    ../../../../../../../code/concurrent/tst/performance/thread_pool_000/main.cpp \
    ../../../../../../../code/concurrent/tst/msa_a.cpp
include (../../../../../common.pri)

LIBS +=-ltenacitas.concurrent.bus -ltenacitas.logger.bus
LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
