
include (../../../common.pri)

QT-=core
TEMPLATE=app
CONFIG+=app
TARGET=tnct.crosswords.app.client

PRJ_DIR=$$BASE_DIR/tnct/crosswords/app/client

#LIBS+=-lpthread

# -lssl -lcrypto


SOURCES = $$PRJ_DIR/main.cpp

# DISTFILES += $$PRJ_DIR/cfg_000.ini


