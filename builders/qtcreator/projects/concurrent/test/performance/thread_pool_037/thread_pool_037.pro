QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.performance.thread_pool_037
CONFIG+=test
SOURCES += \ 
    ../../../../../../../code/concurrent/test/performance/thread_pool_037/main.cpp \ 
    ../../../../../../../code/concurrent/test/msa_a.cpp 
include (../../../../../common.pri)

HEADERS += \ 
     
     
LIBS +=-ltenacitas.concurrent.business -ltenacitas.logger.business
LIBS += -ltenacitas.tester.business
