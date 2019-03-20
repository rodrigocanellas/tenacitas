QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.performance.thread_pool_002
CONFIG+=test
SOURCES += \ 
    ../../../../../../code/concurrent/test/performance/thread_pool_002/main.cpp \ 
    ../../../../../../code/concurrent/test/msa_a.cpp 
include (../../../../common.pri)
LIBS+=-ltenacitas.logger
HEADERS += \ 
     
     
