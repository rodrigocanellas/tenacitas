TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger.tst.cout
include (../../../../common.pri)


SOURCES += \
  $$test_src_dir/logger/cout_log/main.cpp

LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cout.$$static_lib_ext

