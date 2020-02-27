
QT -= qt

TEMPLATE = lib
CONFIG += staticlib

VERSION = 0.0.1

TARGET = tenacitas.options.bus


include (../../../common.pri)

HEADERS += \
  ../../../../../code/pgm_options/bus/pgm_options.h

SOURCES += \
  ../../../../../code/pgm_options/bus/internal/pgm_options.cpp


