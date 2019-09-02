QT -= core
QT -= widgets
QT -= network
QT -= gui
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app
TARGET = tenacitas.concurrent.test.correction.thread_pool_097
CONFIG+=test
SOURCES += \ 
    ../../../../../../code/concurrent/test/correction/thread_pool_097/main.cpp
include (../../../../common.pri)

HEADERS += \ 
    
