QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.business.concurrent


HEADERS += \  \
    ../../../../../code/business/concurrent/dispatcher.h \
    ../../../../../code/business/concurrent/internal/async_loop.h \
    ../../../../../code/business/concurrent/internal/log.h \
    ../../../../../code/business/concurrent/internal/loop.h \
    ../../../../../code/business/concurrent/sleeping_loop.h \
    ../../../../../code/business/concurrent/thread.h \
    ../../../../../code/business/concurrent/thread_pool.h \
    ../../../../../code/business/concurrent/traits.h

include (../../../common.pri)

