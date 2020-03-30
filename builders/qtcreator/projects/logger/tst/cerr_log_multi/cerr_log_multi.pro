TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger.tst.cerr_log_multi


include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
  ../../../../../../code/logger/tst/cerr_log_multi/main.cpp



