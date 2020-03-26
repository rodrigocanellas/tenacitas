TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger.tst.cerr_log_creation




include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS+=$$libs_dir/libtenacitas.logger.cerr.a

SOURCES += \
  ../../../../../../code/logger/tst/cerr_log_creation/main.cpp



