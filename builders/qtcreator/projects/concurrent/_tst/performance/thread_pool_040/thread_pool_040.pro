QT -= core



TEMPLATE = app

TARGET = tenacitas.concurrent._tst.performance.thread_pool_040

CONFIG+=test

SOURCES += \ 
    ../../../../../../../code/concurrent/_tst/performance/thread_pool_040/main.cpp \ 
    ../../../../../../../code/concurrent/_tst/msa_a.cpp 

include (../../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext

HEADERS += \ 
    ../../../../../../../code/concurrent/_tst/msg_a.h \ 
    ../../../../../../../code/concurrent/_tst/performance/thread_pool_tester.h 
