QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = app
TARGET = tenacitas.concurrent._tst.correction.thread_pool_094
CONFIG+=test
SOURCES += \
    ../../../../../../../code/concurrent/_tst/correction/thread_pool_094/main.cpp \
    ../../../../../../../code/concurrent/_tst/msa_a.cpp
include (../../../../../common.pri)

# LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
# LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext
