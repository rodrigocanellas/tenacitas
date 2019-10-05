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
    ../../../../../code/crosswords/business/internal/matrix.h \
    ../../../../../code/crosswords/business/internal/positioning_history.h \
    ../../../../../code/crosswords/business/internal/positions_occupied.h \
    ../../../../../code/crosswords/business/internal/words_references.h \
    ../../../../../code/crosswords/business/positioner000.h \
    ../../../../../code/crosswords/business/positioner001.h

LIBS += -ltenacitas.crosswords.entities


