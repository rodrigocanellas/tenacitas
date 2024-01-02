QT -= core
TEMPLATE = app
TARGET = tenacitas.lib.exp.dispatcher_003
CONFIG += example
include (../../../../../tenacitas.bld/qtcreator/common.pri)
SOURCES = $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/main.cpp

HEADERS = $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/alg.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/cfg.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/dispatcher.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/sto.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/uix.h


LIBS+=-lsqlite3
