QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.pair.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/pair/test/main.cpp

HEADERS += $$BASE_DIR/tenacitas.lib/pair/test/pair_output_test.h
