TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.communication.tst.client_for_file

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/communication/client_for_file/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.communication.$$static_lib_ext

HEADERS += \
  $$test_src_dir/communication/communication/file_connection.h
