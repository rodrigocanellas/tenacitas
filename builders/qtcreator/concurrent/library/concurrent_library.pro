QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.concurrent


HEADERS += \ 
    ../../../../code/concurrent/bus/sleeping_loop.h \
    ../../../../code/concurrent/bus/thread.h \
    ../../../../code/concurrent/bus/thread_pool.h \
    ../../../../code/concurrent/bus/dispatcher.h \
    ../../../../code/concurrent/bus/traits.h \
    ../../../../code/concurrent/bus/internal/loop.h \
    ../../../../code/concurrent/bus/internal/async_loop.h \
    ../../../../code/logger/bus/cerr.h



include (../../common.pri)

SOURCES += \
    ../../../../code/logger/bus/internal/cerr.cpp


