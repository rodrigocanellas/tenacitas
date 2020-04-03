QT -= core
QT -= widgets
QT -= network
QT -= gui



TEMPLATE = app

TARGET = tenacitas.concurrent._tst.correction.dispatcher_002

CONFIG+=test

SOURCES += \
    ../../../../../../../code/concurrent/_tst/correction/dispatcher_002/main.cpp

include (../../../../../common.pri)


# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
