QT -= core
QT -= widgets
QT -= network
QT -= gui



TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.dispatcher_001

CONFIG+=test

SOURCES += \
    ../../../../../../../code/concurrent/tst/correction/dispatcher_001/main.cpp

include (../../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
