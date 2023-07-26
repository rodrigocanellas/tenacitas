QT -= core
TEMPLATE = app
TARGET = tenacitas.lib.tst.translations
CONFIG += test
include (../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES += $$BASE_DIR/tenacitas.lib/tst/translation/main.cpp

DISTFILES += \
  $$BASE_DIR/tenacitas.lib/tst/translation/translation_00.dict

