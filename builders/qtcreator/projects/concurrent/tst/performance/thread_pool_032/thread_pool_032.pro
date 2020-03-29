QT -= core

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.concurrent.tst.performance.thread_pool_032

CONFIG+=test

SOURCES += \ 
    ../../../../../../../code/concurrent/tst/performance/thread_pool_032/main.cpp \ 
    ../../../../../../../code/concurrent/tst/msa_a.cpp 

include (../../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext

HEADERS += \ 
    ../../../../../../../code/concurrent/tst/msg_a.h \ 
    ../../../../../../../code/concurrent/tst/performance/thread_pool_tester.h 
