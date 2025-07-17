QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.format.test

include (../../../common.pri)

SOURCES = $$BASE_DIR_SRC/tnct/format/test/main.cpp

HEADERS = \
        $$BASE_DIR_SRC/tnct/format/test/format_number_test.h
