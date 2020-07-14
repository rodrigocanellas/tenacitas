TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.communication.tst.send_to_file

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/communication/send_to_file/main.cpp

LIBS+=$$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.communication.$$static_lib_ext
