QT -= core
TEMPLATE = app
TARGET = tenacitas.concurrent.tst.thread_pool
CONFIG+=test
include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/concurrent/thread_pool/main.cpp \
  $$test_src_dir/concurrent/msa_a.cpp



LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext



