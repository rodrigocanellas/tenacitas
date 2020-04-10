include (../../../../../common.pri)

QT -= core

TEMPLATE = app

TARGET = tenacitas.concurrent.tst.correction.async_loop_006

CONFIG+=test

SOURCES += \
    $$test_src_dir/concurrent/correction/async_loop_006/main.cpp





LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
