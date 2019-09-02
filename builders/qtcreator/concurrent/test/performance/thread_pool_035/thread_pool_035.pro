QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.performance.thread_pool_035
CONFIG+=test
SOURCES += \ 
    ../../../../../../code/concurrent/test/performance/thread_pool_035/main.cpp \ 
    ../../../../../../code/concurrent/test/msa_a.cpp 
include (../../../../common.pri)

HEADERS += \ 
     
     
