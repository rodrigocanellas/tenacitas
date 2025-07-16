QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tuple.test

include (../../common.pri)


SOURCES = $$BASE_DIR_TST/tnct/tuple/main.cpp

HEADERS += $$BASE_DIR_TST/tnct/tuple/tuple_transform_test.h \
           $$BASE_DIR_TST/tnct/tuple/tuple_traverse_test.h
