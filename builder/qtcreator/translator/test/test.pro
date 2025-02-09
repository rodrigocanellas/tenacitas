QT -= core
TEMPLATE = app
CONFIG += test
TARGET = tnct.translator.test
include (../../common.pri)

SOURCES += $$BASE_DIR/tnct/translator/test/main.cpp

DISTFILES += \
  $$BASE_DIR/tnct/translator/test/translation_00.dict

