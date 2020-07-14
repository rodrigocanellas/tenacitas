TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.concurrent.tst.writer

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/writer/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
