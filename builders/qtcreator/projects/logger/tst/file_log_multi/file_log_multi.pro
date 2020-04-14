TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger._tst.file_log_multi
include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.file.$$static_lib_ext

SOURCES += \
  $$code_src_dir/logger/_tst/file_log_multi/main.cpp



