QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.format

include (../../common.pri)

SOURCES = $$BASE_DIR_TST/tnct/format/main.cpp

HEADERS = \
        $$BASE_DIR_TST/tnct/format/format_number_test.h
