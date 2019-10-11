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
    ../../../../../code/crosswords/business/internal/position_word.h \
    ../../../../../code/crosswords/business/internal/positioning_history.h \
    ../../../../../code/crosswords/business/internal/positions_occupied.h \
    ../../../../../code/crosswords/business/internal/validate_position.h \
    ../../../../../code/crosswords/business/internal/words_by_size.h \
    ../../../../../code/crosswords/business/internal/words_references.h \
    ../../../../../code/crosswords/business/positioner000.h \
    ../../../../../code/crosswords/business/positioner001.h \
    ../../../../../code/crosswords/business/positioner002.h

LIBS += -ltenacitas.crosswords.entities


