QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.crosswords.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/crosswords/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
         $$PRJ_DIR/assembler_test.h  \
         $$PRJ_DIR/dat_test.h  \
         $$PRJ_DIR/grid_creator_test.h  \
         $$PRJ_DIR/organizer_test.h



