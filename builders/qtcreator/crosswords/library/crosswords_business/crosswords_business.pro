QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.crosswords.business



include(../../../common.pri)

HEADERS += \
    ../../../../../code/crosswords/business/internal/log.h \
    ../../../../../code/crosswords/business/positioner000.h

LIBS += -ltenacitas.crosswords.entities


