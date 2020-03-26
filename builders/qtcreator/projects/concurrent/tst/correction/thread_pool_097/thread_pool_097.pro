QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = app
TARGET = tenacitas.concurrent.test.correction.thread_pool_097
CONFIG+=test
SOURCES += \ 
    ../../../../../../../code/concurrent/tst/correction/thread_pool_097/main.cpp
include (../../../../../common.pri)

LIBS +=-ltenacitas.concurrent.bus -ltenacitas.logger.bus
    
LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
