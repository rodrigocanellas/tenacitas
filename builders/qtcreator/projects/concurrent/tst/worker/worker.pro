TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.concurrent.tst.worker

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/worker/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

