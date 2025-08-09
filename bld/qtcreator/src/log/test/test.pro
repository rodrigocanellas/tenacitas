
QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.log.test

include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/log/tst

SOURCES = $$PRJ_DIR/main.cpp

