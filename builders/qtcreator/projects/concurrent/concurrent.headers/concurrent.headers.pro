

#CONFIG -= qt

TEMPLATE = subdirs

#TARGET = tenacitas.concurrent.headers

HEADERS += \  \
    ../../../../../code/concurrent/bus/dispatcher.h \
    ../../../../../code/concurrent/bus/internal/async_loop.h \
    ../../../../../code/concurrent/bus/internal/log.h \
    ../../../../../code/concurrent/bus/internal/loop.h \
    ../../../../../code/concurrent/bus/sleeping_loop.h \
    ../../../../../code/concurrent/bus/thread.h \
    ../../../../../code/concurrent/bus/thread_pool.h \
    ../../../../../code/concurrent/bus/traits.h

include (../../../common.pri)


