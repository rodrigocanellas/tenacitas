include (../../../common.pri)

TEMPLATE = subdirs

HEADERS += \
    $$code_src_dir/concurrent/internal/log.h \
    $$code_src_dir/concurrent/internal/constants.h \
    $$code_src_dir/concurrent/internal/worker_wrapper.h \
    $$code_src_dir/concurrent/thread.h \
    $$code_src_dir/concurrent/timeout_callback.h \
    $$code_src_dir/concurrent/breaker.h \
    $$code_src_dir/concurrent/runner.h \
    $$code_src_dir/concurrent/async_loop.h \
    $$code_src_dir/concurrent/circular_fixed_size_queue.h \
    $$code_src_dir/concurrent/circular_unlimited_size_queue.h \
    $$code_src_dir/concurrent/sleeping_loop.h    \
    $$code_src_dir/concurrent/producer_consumer.h \
    $$code_src_dir/concurrent/queue.h \
    $$code_src_dir/calendar/epoch.h   \
    $$code_src_dir/concurrent/dispatcher.h




#    $$code_src_dir/concurrent/reader.h \
#    $$code_src_dir/concurrent/writer.h \






