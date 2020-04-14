TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger._tst.clog_log_how_to
include (../../../../_common.pri)



# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.clog.$$static_lib_ext

SOURCES += \
  $$code_src_dir/logger/_tst/clog_log_how_to/main.cpp





