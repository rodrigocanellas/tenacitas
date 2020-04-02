TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.<PROJECT>.tst.<TST000>

include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.tester.$$static_lib_ext
