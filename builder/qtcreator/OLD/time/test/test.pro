QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.time.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tnct/time/test/main.cpp

HEADERS += $$BASE_DIR/tnct/time/test/chrono_output_test.h
