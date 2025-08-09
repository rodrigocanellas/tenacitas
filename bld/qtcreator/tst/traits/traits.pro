QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.tst.traits

include (../../common.pri)



SOURCES = $$BASE_DIR/tenacitas/tst/traits/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas/tst/traits/string_test.h \
        $$BASE_DIR/tenacitas/tst/traits/is_tuple_test.h \
        $$BASE_DIR/tenacitas/tst/traits/tuple_like_test.h \
        $$BASE_DIR/tenacitas/tst/traits/tuple_find_test.h \
        $$BASE_DIR/tenacitas/tst/traits/is_type_in_tuple_test.h \
        $$BASE_DIR/tenacitas/tst/traits/tuple_contains_tuple_test.h \
        $$BASE_DIR/tenacitas/tst/traits/tuple_traverse_test.h

        # $$BASE_DIR/tenacitas/tst/traits/async_test.h \

