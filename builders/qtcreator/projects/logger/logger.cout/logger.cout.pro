CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.cout





include (../../../common.pri)

HEADERS += \
  ../../../../../code/logger/bus/level.h \
  ../../../../../code/logger/bus/log_t.h \
  ../../../../../code/logger/cout/log.h

SOURCES += \
  ../../../../../code/logger/cout/internal/log.cpp
