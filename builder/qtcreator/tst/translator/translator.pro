QT -= core
TEMPLATE = app
CONFIG += test
TARGET = tnct.translator.test
include (../../common.pri)

SOURCES += $$BASE_DIR_TST/tnct/translator/main.cpp

DISTFILES += \
  $$BASE_DIR_TST/tnct/translator/translation_00.dict

