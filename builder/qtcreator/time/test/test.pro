QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.time.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/time/test/main.cpp

HEADERS += $$BASE_DIR/tenacitas.lib/time/test/chrono_output_test.h
