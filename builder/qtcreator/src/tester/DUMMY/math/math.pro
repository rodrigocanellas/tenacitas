QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.test.math

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib/test/math/main.cpp

HEADERS= $$BASE_DIR/tenacitas.lib/test/math/factorial.h

