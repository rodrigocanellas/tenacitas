CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.status

include (../../../common.pri)

HEADERS += \
  $$code_src_dir/status/result.h \
  $$code_src_dir/status/translations.h

SOURCES += \
  $$code_src_dir/status/internal/translations.cpp

