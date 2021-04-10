TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger.tst.cerr
include (../../../../../common.pri)


#LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
#LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
  $$test_src_dir/tenacitas/logger/cerr_log/main.cpp



