QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.correction.thread_pool_092
CONFIG+=test
SOURCES += \ 
    ../../../../../../../code/concurrent/test/correction/thread_pool_092/main.cpp \
    ../../../../../../../code/concurrent/test/msa_a.cpp 
include (../../../../../common.pri)

LIBS +=-ltenacitas.concurrent.business -ltenacitas.logger.business
    
LIBS += -ltenacitas.tester.business
