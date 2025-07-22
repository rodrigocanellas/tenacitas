QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.generic

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas/tst/generic/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas/tst/generic/tester_test.h \
        $$BASE_DIR/tenacitas/tst/generic/id_test.h \
        $$BASE_DIR/tenacitas/tst/generic/format_number_test.h
