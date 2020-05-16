QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.dispatcher
CONFIG+=test
include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/dispatcher/main.cpp



LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
