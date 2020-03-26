CONFIG -= qt

TEMPLATE = lib

TARGET = tenacitas.options



include (../../../../common.pri)

HEADERS += \
  ../../../../../../code/pgm_options/bus/pgm_options.h

SOURCES += \
  ../../../../../../code/pgm_options/bus/internal/pgm_options.cpp
