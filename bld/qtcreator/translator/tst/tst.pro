QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.translator.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/translator/tst


SOURCES = $$PRJ_DIR/main.cpp

DISTFILES += \
  $$PRJ_DIR/translation_00.dict




