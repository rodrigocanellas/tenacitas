QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.string.test

include (../../common.pri)


HEADERS += $$BASE_DIR/tnct/string/test/fixed_size_string_test.h
SOURCES = $$BASE_DIR/tnct/string/test/main.cpp


