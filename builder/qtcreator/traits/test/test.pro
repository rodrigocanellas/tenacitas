QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.traits.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/traits/test/main.cpp

HEADERS = \
        $$BASE_DIR/tenacitas.lib/traits/test/subscriber_test.h
        # \
        # $$BASE_DIR/tenacitas.lib/traits/test/queue.h
