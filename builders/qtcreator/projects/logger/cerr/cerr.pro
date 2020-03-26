CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.cerr



HEADERS += \
    ../../../../../code/logger/bus/level.h \
    ../../../../../code/logger/bus/log.h \
    ../../../../../code/logger/cerr/cerr.h

SOURCES += \
    ../../../../../code/logger/cerr/internal/cerr.cpp

include (../../../common.pri)


