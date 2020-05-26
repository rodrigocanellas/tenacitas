TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.communication.tst.tcp_socket

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/communication/tcp_socket/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.communication.$$static_lib_ext
