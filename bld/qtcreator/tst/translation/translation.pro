QT -= core
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.lib.tst.translations
include (../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES += $$BASE_DIR/tenacitas.lib/tst/translation/main.cpp

DISTFILES += \
  $$BASE_DIR/tenacitas.lib/tst/translation/translation_00.dict

