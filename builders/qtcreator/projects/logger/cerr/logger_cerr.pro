CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.cerr



HEADERS += \
    ../../../../../code/logger/bus/level.h \
    ../../../../../code/logger/bus/log_t.h \
    ../../../../../code/logger/cerr/log.h

SOURCES += \
  ../../../../../code/logger/cerr/internal/log.cpp

include (../../../common.pri)


