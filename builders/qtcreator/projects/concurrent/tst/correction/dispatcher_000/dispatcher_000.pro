QT -= core
QT -= widgets
QT -= network
QT -= gui



TEMPLATE = app

TARGET = tenacitas.concurrent.tst.correction.dispatcher_000

CONFIG+=test

SOURCES += \
    $$test_src_dir/concurrent/correction/dispatcher_000/main.cpp

include (../../../../../common.pri)


# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext