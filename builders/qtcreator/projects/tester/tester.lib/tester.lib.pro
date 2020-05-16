CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.tester

include (../../../common.pri)

HEADERS += \
  $$code_src_dir/tester/test.h \
  $$code_src_dir/program/options.h

SOURCES += \
  $$code_src_dir/tester/internal/test.cpp \
  $$code_src_dir/program/internal/options.cpp

#LIBS += $$libs_dir/libtenacitas.program.$$static_lib_ext
