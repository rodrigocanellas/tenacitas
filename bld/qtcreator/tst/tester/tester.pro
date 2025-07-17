QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.tester

include (../../common.pri)

SOURCES = $$BASE_DIR_TST/tnct/tester/main.cpp

HEADERS = \
        $$BASE_DIR_TST/tnct/tester/test_test.h
