TEMPLATE = app
CONFIG -= qt
CONFIG+=test
TARGET = tenacitas.logger.tst.cout
include (../../../../../common.pri)


SOURCES += \
  $$test_src_dir/logger/cout_log/main.cpp

