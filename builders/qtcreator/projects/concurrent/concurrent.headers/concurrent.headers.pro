

#CONFIG -= qt

TEMPLATE = subdirs

#TARGET = tenacitas.concurrent.headers

HEADERS += \  \
    ../../../../../code/concurrent/_bus/dispatcher.h \
    ../../../../../code/concurrent/_bus/internal/async_loop.h \
    ../../../../../code/concurrent/_bus/internal/log.h \
    ../../../../../code/concurrent/_bus/internal/loop.h \
    ../../../../../code/concurrent/_bus/sleeping_loop.h \
    ../../../../../code/concurrent/_bus/thread.h \
    ../../../../../code/concurrent/_bus/thread_pool.h \
    ../../../../../code/concurrent/_bus/traits.h

include (../../../common.pri)


