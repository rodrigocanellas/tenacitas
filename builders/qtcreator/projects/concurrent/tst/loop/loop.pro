TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.concurrent.tst.loop

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/loop/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.concurrent.$$static_lib_ext
