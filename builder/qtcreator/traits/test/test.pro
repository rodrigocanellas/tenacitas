QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.traits.test

include (../../common.pri)

TRAITS_TEST=$$BASE_DIR/tnct/traits/test

SOURCES = $$TRAITS_TEST/main.cpp

HEADERS = \
        $$TRAITS_TEST/subscriber_test.h \
        $$TRAITS_TEST/is_tuple_test.h \
        $$TRAITS_TEST/tuple_like_test.h \
        $$TRAITS_TEST/tuple_find_test.h \
        $$TRAITS_TEST/is_type_in_tuple_test.h \
        $$TRAITS_TEST/tuple_contains_tuple_test.h \
        $$TRAITS_TEST/dispatcher_test.h

