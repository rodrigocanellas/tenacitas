TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger.tst.clog_log_how_to
include (../../../../common.pri)



LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.clog.$$static_lib_ext

SOURCES += \
  $$test_src_dir/logger/clog_log/main.cpp





