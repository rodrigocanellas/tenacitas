CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.concurrent
include (../../../common.pri)


HEADERS += \
    $$code_src_dir/concurrent/internal/log.h \
    $$code_src_dir/concurrent/loop.h \
    $$code_src_dir/concurrent/traits.h \
    $$code_src_dir/concurrent/result.h \
    $$code_src_dir/concurrent/internal/process.h \
    $$code_src_dir/concurrent/thread.h \
    $$code_src_dir/concurrent/async_loop.h

#    $$code_src_dir/concurrent/dispatcher.h \

#    $$code_src_dir/concurrent/sleeping_loop.h \

#    $$code_src_dir/concurrent/thread_pool.h \





