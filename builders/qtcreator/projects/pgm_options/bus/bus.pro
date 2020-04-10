
QT -= qt

TEMPLATE = lib
CONFIG += staticlib

VERSION = 0.0.1

TARGET = tenacitas.options.bus


include (../../../common.pri)

HEADERS += \
  $$code_src_dir/pgm_options/bus/pgm_options.h

SOURCES += \
  $$code_src_dir/pgm_options/bus/internal/pgm_options.cpp


