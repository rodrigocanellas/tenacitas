QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tuple.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/tuple/test/main.cpp

HEADERS += $$BASE_DIR/tenacitas.lib/tuple/test/is_tuple_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/tuple_like_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/tuple_find_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/is_type_in_tuple_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/tuple_contains_tuple_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/tuple_transform_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/tuple_traverse_test.h \
           $$BASE_DIR/tenacitas.lib/tuple/test/tuple_output_test.h
