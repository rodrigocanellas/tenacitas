TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger._tst.cout_log_multi


include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cout.$$static_lib_ext

SOURCES += \
  ../../../../../../code/logger/_tst/cout_log_multi/main.cpp



