CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.clog





include (../../../common.pri)

HEADERS += \
  ../../../../../code/logger/bus/level.h \
  ../../../../../code/logger/bus/t_log.h \
  ../../../../../code/logger/clog/clog.h

SOURCES += \
  ../../../../../code/logger/clog/internal/clog.cpp


