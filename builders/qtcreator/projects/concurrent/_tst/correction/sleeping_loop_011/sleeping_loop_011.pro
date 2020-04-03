QT -= core
QT -= widgets
QT -= network
QT -= gui



TEMPLATE = app

TARGET = tenacitas.concurrent._tst.correction.sleeping_loop_011

CONFIG+=test

HEADERS +=

SOURCES += \
    ../../../../../../../code/concurrent/_tst/correction/sleeping_loop_011/main.cpp

include (../../../../../common.pri)

# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
