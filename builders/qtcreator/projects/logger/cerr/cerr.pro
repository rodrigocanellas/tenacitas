CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.cerr



HEADERS += \
    ../../../../../../../code/logger/bus/level.h \
    ../../../../../../../code/logger/bus/t_log.h \
    ../../../../../../../code/logger/cerr/log.h

SOURCES += \
  ../../../../../../../code/logger/cerr/internal/log.cpp

include (../../../../../common.pri)


