
QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.log

include (../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES = $$TST_DIR/log/main.cpp

