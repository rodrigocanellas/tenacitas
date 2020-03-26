TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger.tst.cout_log_how_to

SOURCES += \
  ../../../../../../code/logger/tst/cout_log_how_to/main.cpp

include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cout.$$static_lib_ext

#LIBS+=$$libs_dir/libtenacitas.logger.cerr.a


