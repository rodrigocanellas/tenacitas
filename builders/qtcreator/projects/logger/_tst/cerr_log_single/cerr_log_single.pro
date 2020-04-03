TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger._tst.cerr_log_single


include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
  ../../../../../../code/logger/_tst/cerr_log_single/main.cpp


