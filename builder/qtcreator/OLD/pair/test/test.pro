QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.pair.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tnct/pair/test/main.cpp

HEADERS += $$BASE_DIR/tnct/pair/test/pair_output_test.h
