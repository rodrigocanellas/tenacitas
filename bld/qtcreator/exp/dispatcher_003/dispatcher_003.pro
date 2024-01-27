QT -= core
TEMPLATE = app
TARGET = tenacitas.lib.exp.dispatcher_003
CONFIG += example
QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)
SOURCES = $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/main.cpp

HEADERS = $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/dom/fun.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/cfg/cfg.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/per/per.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/uix/uix.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/dom/dat.h \
          $$BASE_DIR/tenacitas.lib/exp/dispatcher_003/asy/asy.h


LIBS+=-lsqlite3
