QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.generic.test

include (../../common.pri)

SOURCES = $$BASE_DIR_TST/tnct/generic/main.cpp

HEADERS = \
        $$BASE_DIR_TST/tnct/generic/tester_test.h \
        $$BASE_DIR_TST/tnct/generic/id_test.h \
        $$BASE_DIR_TST/tnct/generic/format_number_test.h
