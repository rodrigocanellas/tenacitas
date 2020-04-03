TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger._tst.file_log_single


include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.file.$$static_lib_ext

SOURCES += \
  ../../../../../../code/logger/_tst/file_log_single/main.cpp


