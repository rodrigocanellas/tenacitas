CONFIG -= qt

TEMPLATE = lib

TARGET = tenacitas.options



include (../../../../common.pri)

HEADERS += \
  $$code_src_dir/pgm_options/bus/pgm_options.h

SOURCES += \
  $$code_src_dir/pgm_options/bus/internal/pgm_options.cpp
