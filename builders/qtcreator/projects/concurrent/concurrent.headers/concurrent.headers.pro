include (../../../common.pri)

TEMPLATE = subdirs

HEADERS += \  \
    $$code_src_dir/concurrent/dispatcher.h \
    $$code_src_dir/concurrent/internal/async_loop.h \
    $$code_src_dir/concurrent/internal/log.h \
    $$code_src_dir/concurrent/internal/loop.h \
    $$code_src_dir/concurrent/sleeping_loop.h \
    $$code_src_dir/concurrent/thread.h \
    $$code_src_dir/concurrent/thread_pool.h \
    $$code_src_dir/concurrent/traits.h




