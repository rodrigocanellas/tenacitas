QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.traits.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/traits/tst



SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
        $$PRJ_DIR/string_test.h \
        $$PRJ_DIR/is_tuple_test.h \
        $$PRJ_DIR/tuple_like_test.h \
        $$PRJ_DIR/tuple_find_test.h \
        $$PRJ_DIR/is_type_in_tuple_test.h \
        $$PRJ_DIR/tuple_contains_tuple_test.h \
        $$PRJ_DIR/tuple_traverse_test.h




