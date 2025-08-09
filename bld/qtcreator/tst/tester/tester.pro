QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.tst.tester

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas/tst/tester/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas/tst/tester/test_test.h
