
include (../../../common.pri)

QT += core gui widgets

TEMPLATE=app
CONFIG+=app
TARGET=tnct.crosswords.app.server

PRJ_DIR=$$BASE_DIR/tnct/crosswords/app/server

LIBS+=-lpthread

# -lssl -lcrypto

HEADERS = $$PRJ_DIR/connection.h \
          $$PRJ_DIR/statistics.h

SOURCES = $$PRJ_DIR/main.cpp

# DISTFILES += $$PRJ_DIR/cfg_000.ini


LIBS+= -L/usr/local/lib -lixwebsocket  -lssl -lcrypto -lpthread -lz
