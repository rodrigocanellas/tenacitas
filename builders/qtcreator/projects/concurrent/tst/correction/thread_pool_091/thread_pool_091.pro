include (../../../../../common.pri)

QT -= core

TEMPLATE = app
TARGET = tenacitas.concurrent.tst.correction.thread_pool_091
CONFIG+=test
SOURCES += \
    $$test_src_dir/concurrent/correction/thread_pool_091/main.cpp \
    $$test_src_dir/concurrent/msa_a.cpp


# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
