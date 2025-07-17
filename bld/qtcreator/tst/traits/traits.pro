QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.traits

include (../../common.pri)



SOURCES = $$BASE_DIR_TST/tnct/traits/main.cpp

HEADERS = \
        $$BASE_DIR_TST/tnct/traits/async_test.h \
        $$BASE_DIR_TST/tnct/traits/string_test.h \
        $$BASE_DIR_TST/tnct/traits/is_tuple_test.h \
        $$BASE_DIR_TST/tnct/traits/tuple_like_test.h \
        $$BASE_DIR_TST/tnct/traits/tuple_find_test.h \
        $$BASE_DIR_TST/tnct/traits/is_type_in_tuple_test.h \
        $$BASE_DIR_TST/tnct/traits/tuple_contains_tuple_test.h \
        $$BASE_DIR_TST/tnct/traits/tuple_traverse_test.h

