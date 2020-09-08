include (../../../common.pri)

TEMPLATE = subdirs

HEADERS += \
    $$code_src_dir/concurrent/internal/log.h \
    $$code_src_dir/concurrent/thread.h \
    $$code_src_dir/concurrent/traits.h \
    $$code_src_dir/concurrent/executer.h  \
    $$code_src_dir/concurrent/loop.h  \
    $$code_src_dir/concurrent/async_loop.h \
    $$code_src_dir/concurrent/sleeping_loop.h    \
    $$code_src_dir/concurrent/fixed_size_queue.h
#    \
#    $$code_src_dir/concurrent/producer_consumer.h \
#    $$code_src_dir/concurrent/dispatcher.h \


#    $$code_src_dir/concurrent/processor.h \
#    $$code_src_dir/concurrent/result.h \
#    $$code_src_dir/concurrent/thread_pool.h \
#    $$code_src_dir/concurrent/dispatcher.h \
#    $$code_src_dir/concurrent/reader.h \
#    $$code_src_dir/concurrent/writer.h \






