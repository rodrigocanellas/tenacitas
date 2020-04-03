CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.clog





include (../../../common.pri)

HEADERS += \
  ../../../../../code/logger/_bus/level.h \
  ../../../../../code/logger/_bus/log_t.h \
  ../../../../../code/logger/clog/log.h

SOURCES += \
  ../../../../../code/logger/clog/internal/log.cpp


