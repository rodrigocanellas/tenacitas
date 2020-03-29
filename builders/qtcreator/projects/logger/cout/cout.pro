CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.cout





include (../../../../../common.pri)

HEADERS += \
  ../../../../../../../code/logger/bus/level.h \
  ../../../../../../../code/logger/bus/t_log.h \
  ../../../../../../../code/logger/cout/cout.h

SOURCES += \
  ../../../../../../../code/logger/cout/internal/cout.cpp
