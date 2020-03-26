QT -= core
QT -= widgets
QT -= network
QT -= gui



TEMPLATE = app

TARGET = tenacitas.concurrent.test.correction.async_loop_000

CONFIG+=test


SOURCES += \
    ../../../../../../../code/concurrent/tst/correction/async_loop_000/main.cpp

include (../../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext

HEADERS += \
  ../../../../../../../code/concurrent/tst/concurrent_log.h
