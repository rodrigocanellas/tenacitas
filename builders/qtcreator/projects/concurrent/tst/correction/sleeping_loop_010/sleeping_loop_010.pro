include (../../../../../common.pri)

QT -= core

TEMPLATE = app

TARGET = tenacitas.concurrent.tst.correction.sleeping_loop_010

CONFIG+=test

HEADERS +=

SOURCES += \
    $$test_src_dir/concurrent/correction/sleeping_loop_010/main.cpp



# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
