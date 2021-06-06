TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.concurrent.tst.executer

include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/executer/main.cpp

