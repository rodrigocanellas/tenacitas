QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tuple.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tnct/tuple/test/main.cpp

HEADERS += $$BASE_DIR/tnct/tuple/test/tuple_transform_test.h \
           $$BASE_DIR/tnct/tuple/test/tuple_traverse_test.h
