QT -= core
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.lib.translator.test
include (../../common.pri)

SOURCES += $$BASE_DIR/tenacitas.lib/translator/test/main.cpp

DISTFILES += \
  $$BASE_DIR/tenacitas.lib/translator/test/translation_00.dict

