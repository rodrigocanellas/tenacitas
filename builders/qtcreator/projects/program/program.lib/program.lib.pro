
QT -= qt

TEMPLATE = lib
CONFIG += staticlib

VERSION = 0.0.1

TARGET = tenacitas.program

include (../../../common.pri)

HEADERS += \
  $$code_src_dir/program/options.h

SOURCES += \
  $$code_src_dir/program/internal/options.cpp

LIBS += $$libs_dir/libtenacitas.program.$$static_lib_ext


