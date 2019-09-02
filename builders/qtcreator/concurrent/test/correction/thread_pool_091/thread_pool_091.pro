QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.correction.thread_pool_091
CONFIG+=test
SOURCES += \ 
    ../../../../../../code/concurrent/test/correction/thread_pool_091/main.cpp \
    ../../../../../../code/concurrent/test/msa_a.cpp 
include (../../../../common.pri)

HEADERS += \ 
    
