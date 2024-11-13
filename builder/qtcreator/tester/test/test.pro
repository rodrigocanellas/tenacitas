QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tester.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/tester/test/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas.lib/tester/test/test_test.h
