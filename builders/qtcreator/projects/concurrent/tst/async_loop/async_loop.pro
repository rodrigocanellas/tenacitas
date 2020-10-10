QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.async_loop
CONFIG+=test
include (../../../../common.pri)

SOURCES += \
    $$test_src_dir/concurrent/async_loop/async_loop_tests.cpp



LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext


