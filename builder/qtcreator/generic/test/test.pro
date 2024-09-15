QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.generic.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/generic/test/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas.lib/generic/test/tester_test.h \
        $$BASE_DIR/tenacitas.lib/generic/test/id_test.h \
        $$BASE_DIR/tenacitas.lib/generic/test/format_number_test.h
