QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.thread_pool_000
CONFIG+=test
include (../../../../../common.pri)

SOURCES += \
    $$test_src_dir/concurrent/performance/thread_pool_000/main.cpp \
    $$test_src_dir/concurrent/msa_a.cpp

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext

HEADERS += \
    $$test_src_dir/concurrent/msg_a.h \
    $$test_src_dir/concurrent/performance/thread_pool_tester.h
