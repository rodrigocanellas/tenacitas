QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.concurrent.business


HEADERS += \  \
    ../../../../../code/concurrent/business/dispatcher.h \
    ../../../../../code/concurrent/business/internal/async_loop.h \
    ../../../../../code/concurrent/business/internal/log.h \
    ../../../../../code/concurrent/business/internal/loop.h \
    ../../../../../code/concurrent/business/sleeping_loop.h \
    ../../../../../code/concurrent/business/thread.h \
    ../../../../../code/concurrent/business/thread_pool.h \
    ../../../../../code/concurrent/business/traits.h \
    ../../../../../code/string/business/file_name_from_path.h

include (../../../common.pri)

