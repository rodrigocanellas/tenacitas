include (../../../common.pri)

TEMPLATE = subdirs

HEADERS += \
    $$code_src_dir/calendar/epoch.h \
    $$code_src_dir/concurrent/internal/log.h \
    $$code_src_dir/concurrent/internal/constants.h \
    $$code_src_dir/concurrent/internal/function_wrapper.h   \
    $$code_src_dir/concurrent/time_unit.h \
    $$code_src_dir/concurrent/thread.h \
    $$code_src_dir/concurrent/timeout_callback.h \
    $$code_src_dir/concurrent/executer.h \
    $$code_src_dir/concurrent/breaker.h
#    \
#    $$code_src_dir/concurrent/async_loop.h
#    \
#    $$code_src_dir/concurrent/queue.h \
#    $$code_src_dir/concurrent/circular_fixed_size_queue.h \
#    $$code_src_dir/concurrent/circular_unlimited_size_queue.h  \
#    $$code_src_dir/concurrent/sleeping_loop.h \
#    $$code_src_dir/concurrent/worker.h \
#    $$code_src_dir/concurrent/workers.h




#    $$code_src_dir/concurrent/reader.h \
#    $$code_src_dir/concurrent/writer.h \






