TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger._tst.cout_log_how_to
include (../../../../common.pri)


SOURCES += \
  $$code_src_dir/logger/_tst/cout_log_how_to/main.cpp



# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cout.$$static_lib_ext

#LIBS+=$$libs_dir/libtenacitas.logger.cerr.a


