QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.tst.tuple

include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/tst/tuple

SOURCES = $$PRJ_DIR/main.cpp

HEADERS += $$PRJ_DIR/tuple_transform_test.h \
           $$PRJ_DIR/tuple_traverse_test.h
