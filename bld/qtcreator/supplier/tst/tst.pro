QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.supplier.tst

include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/supplier/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/cpt_test.h
