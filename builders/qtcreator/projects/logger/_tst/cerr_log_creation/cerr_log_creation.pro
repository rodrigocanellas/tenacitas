TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger._tst.cerr_log_creation
include (../../../../common.pri)





# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
  $$code_src_dir/logger/_tst/cerr_log_creation/main.cpp



