QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.tuple

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas/tst/tuple/main.cpp

HEADERS += $$BASE_DIR/tenacitas/tst/tuple/tuple_transform_test.h \
           $$BASE_DIR/tenacitas/tst/tuple/tuple_traverse_test.h
