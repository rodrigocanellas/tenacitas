TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger.file_log
include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.file.$$static_lib_ext
#LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
  $$test_src_dir/logger/file_log/main.cpp





