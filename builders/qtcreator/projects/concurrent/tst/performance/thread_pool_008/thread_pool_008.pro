QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = app
TARGET = tenacitas.concurrent.test.performance.thread_pool_008
CONFIG+=test
SOURCES += \ 
    ../../../../../../../code/concurrent/tst/performance/thread_pool_008/main.cpp \ 
    ../../../../../../../code/concurrent/tst/msa_a.cpp 
include (../../../../../common.pri)

HEADERS += \ 
     
     
LIBS +=-ltenacitas.concurrent.bus -ltenacitas.logger.bus
LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
