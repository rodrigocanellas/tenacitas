
QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.sleeping_loop
CONFIG+=test
include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/sleeping_loop/main.cpp



LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext


