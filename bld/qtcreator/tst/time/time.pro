QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.tst.time

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas/tst/time/main.cpp

HEADERS += $$BASE_DIR/tenacitas/tst/time/chrono_output_test.h
