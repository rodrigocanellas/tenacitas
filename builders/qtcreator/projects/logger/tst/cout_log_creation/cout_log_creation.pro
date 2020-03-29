TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.logger.tst.cout_log_creation


include (../../../../../common.pri)


SOURCES += \
  ../../../../../../../../code/logger/tst/cout_log_creation/main.cpp

LIBS += $$$libs_dir/libtenacitas.tester.lib.$$$static_lib_ext
LIBS+=$$$libs_dir/libtenacitas.logger.cout.a

