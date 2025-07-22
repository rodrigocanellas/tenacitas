QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.string

include (../../common.pri)


HEADERS += $$BASE_DIR/tenacitas/tst/string/fixed_size_string_test.h
SOURCES = $$BASE_DIR/tenacitas/tst/string/main.cpp


