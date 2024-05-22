QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.generic.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/generic/test/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas.lib/generic/test/tester.h \
        $$BASE_DIR/tenacitas.lib/generic/test/id.h \
        $$BASE_DIR/tenacitas.lib/generic/test/format_number.h
