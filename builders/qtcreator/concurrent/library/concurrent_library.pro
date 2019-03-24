QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.concurrent


HEADERS += \ 
    ../../../../code/concurrent/business/sleeping_loop.h \
    ../../../../code/concurrent/business/thread.h \
    ../../../../code/concurrent/business/thread_pool.h \
    ../../../../code/concurrent/business/dispatcher.h \
    ../../../../code/concurrent/business/traits.h \
    ../../../../code/concurrent/business/internal/loop.h \
    ../../../../code/concurrent/business/internal/async_loop.h \
    ../../../../code/logger/business/cerr.h



include (../../common.pri)

SOURCES += \
    ../../../../code/logger/business/internal/cerr.cpp


