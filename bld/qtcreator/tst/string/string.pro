QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.string

include (../../common.pri)


HEADERS += $$BASE_DIR_TST/tnct/string/fixed_size_string_test.h
SOURCES = $$BASE_DIR_TST/tnct/string/main.cpp


