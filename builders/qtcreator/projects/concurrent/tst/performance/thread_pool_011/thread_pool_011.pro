QT -= core

include (../../../../../common.pri)

TEMPLATE = app

TARGET = tenacitas.concurrent.tst.performance.thread_pool_011

CONFIG+=test

SOURCES += \
    $$test_src_dir/concurrent/performance/thread_pool_011/main.cpp \
    $$test_src_dir/concurrent/msa_a.cpp



LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext

HEADERS += \
    $$test_src_dir/concurrent/msg_a.h \
    $$test_src_dir/concurrent/performance/thread_pool_tester.h
