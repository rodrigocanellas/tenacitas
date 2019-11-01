QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.performance.thread_pool_030
CONFIG+=test
SOURCES += \ 
    ../../../../../../../code/test/concurrent/performance/thread_pool_030/main.cpp \ 
    ../../../../../../../code/test/concurrent/msa_a.cpp 
include (../../../../../common.pri)

HEADERS += \ 
     
     
LIBS+=-ltenacitas.business.concurrent -ltenacitas.business.logger
LIBS += -ltenacitas.business.tester
