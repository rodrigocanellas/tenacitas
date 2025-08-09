QT -= core
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.tst.translator
include (../../common.pri)

SOURCES += $$BASE_DIR/tenacitas/tst/translator/main.cpp

DISTFILES += \
  $$BASE_DIR/tenacitas/tst/translator/translation_00.dict

