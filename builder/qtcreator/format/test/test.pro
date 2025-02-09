QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.format.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/format/test/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas.lib/format/test/format_number_test.h
