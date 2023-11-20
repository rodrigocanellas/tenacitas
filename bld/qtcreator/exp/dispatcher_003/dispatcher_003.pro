QT -= core
TEMPLATE = app
TARGET = tenacitas.lib.exp.dispatcher_003
CONFIG += example
include (../../../../../tenacitas.bld/qtcreator/common.pri)
SOURCES = $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/main.cpp



LIBS+=-lsqlite3
