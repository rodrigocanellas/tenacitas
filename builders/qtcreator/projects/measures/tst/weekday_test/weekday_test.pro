TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.measures.tst.weekday_test

include (../../../../common.pri)

SOURCES += \
  $$test_src_dir/measures/weekday_test/main.cpp

LIBS += $$libs_dir/libtenacitas.measures.time.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext