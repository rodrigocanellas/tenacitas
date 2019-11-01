QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.business.crosswords



include(../../../common.pri)

HEADERS += \
    ../../../../../code/business/crosswords/internal/log.h \
    ../../../../../code/business/crosswords/internal/matrix.h \
    ../../../../../code/business/crosswords/internal/position_word.h \
    ../../../../../code/business/crosswords/internal/positioner000.h \
    ../../../../../code/business/crosswords/internal/positioner001.h \
    ../../../../../code/business/crosswords/internal/positioner002.h \
    ../../../../../code/business/crosswords/internal/positioner003.h \
    ../../../../../code/business/crosswords/internal/positioner004.h \
    ../../../../../code/business/crosswords/internal/positioner005.h \
    ../../../../../code/business/crosswords/internal/positioner006.h \
    ../../../../../code/business/crosswords/internal/positioning_history.h \
    ../../../../../code/business/crosswords/internal/positions_occupied.h \
    ../../../../../code/business/crosswords/internal/validate_position.h \
    ../../../../../code/business/crosswords/internal/word_positioner.h \
    ../../../../../code/business/crosswords/internal/words_by_size.h \
    ../../../../../code/business/crosswords/internal/words_positioner.h \
    ../../../../../code/business/crosswords/internal/words_positioner_group.h \
    ../../../../../code/business/crosswords/internal/words_references.h \
    ../../../../../code/business/crosswords/positioner.h \
    ../../../../../code/business/crosswords/internal/positioner000.h \
    ../../../../../code/business/crosswords/internal/positioner001.h \
    ../../../../../code/business/crosswords/internal/positioner002.h \
    ../../../../../code/business/crosswords/internal/positioner003.h \
    ../../../../../code/business/crosswords/internal/positioner004.h

LIBS += -ltenacitas.entities.crosswords


