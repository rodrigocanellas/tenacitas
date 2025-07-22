QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.format

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas/tst/format/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas/tst/format/format_number_test.h
