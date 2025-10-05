include (../../common.pri)

QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.io.tst


PRJ_DIR=$$BASE_DIR/tnct/io/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
         $$PRJ_DIR/cpt_test.h


