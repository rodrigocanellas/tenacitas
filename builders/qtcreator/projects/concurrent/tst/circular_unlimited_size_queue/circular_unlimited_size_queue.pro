TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.concurrent.tst.circular_unlimited_size_queue

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/circular_unlimited_size_queue/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

