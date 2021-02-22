QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.async_loop
CONFIG+=test
include (../../../../../common.pri)
SOURCES += \
    $$test_src_dir/concurrent/async_loop/main.cpp



