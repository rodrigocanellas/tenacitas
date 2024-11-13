QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.traits.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/traits/test/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas.lib/traits/test/subscriber_test.h \
        $$BASE_DIR/tenacitas.lib/traits/test/is_tuple_test.h \
        $$BASE_DIR/tenacitas.lib/traits/test/tuple_like_test.h \
        $$BASE_DIR/tenacitas.lib/traits/test/tuple_find_test.h \
        $$BASE_DIR/tenacitas.lib/traits/test/is_type_in_tuple_test.h \
        $$BASE_DIR/tenacitas.lib/traits/test/tuple_contains_tuple_test.h
