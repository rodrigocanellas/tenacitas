TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.concurrent.tst.message_queue

include (../../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/message_queue/main.cpp


