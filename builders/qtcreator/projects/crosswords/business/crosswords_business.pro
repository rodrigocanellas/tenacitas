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
    ../../../../../code/crosswords/business/internal/positioner000.h \
    ../../../../../code/crosswords/business/internal/positioner001.h \
    ../../../../../code/crosswords/business/internal/positioner002.h \
    ../../../../../code/crosswords/business/internal/positioner003.h \
    ../../../../../code/crosswords/business/internal/positioner004.h \
    ../../../../../code/crosswords/business/internal/positioner005.h \
    ../../../../../code/crosswords/business/internal/positioner006.h \
    ../../../../../code/crosswords/business/internal/positioning_history.h \
    ../../../../../code/crosswords/business/internal/positions_occupied.h \
    ../../../../../code/crosswords/business/internal/validate_position.h \
    ../../../../../code/crosswords/business/internal/word_positioner.h \
    ../../../../../code/crosswords/business/internal/words_by_size.h \
    ../../../../../code/crosswords/business/internal/words_positioner.h \
    ../../../../../code/crosswords/business/internal/words_positioner_group.h \
    ../../../../../code/crosswords/business/internal/words_references.h \
    ../../../../../code/crosswords/business/positioner.h \
    ../../../../../code/crosswords/business/internal/positioner000.h \
    ../../../../../code/crosswords/business/internal/positioner001.h \
    ../../../../../code/crosswords/business/internal/positioner002.h \
    ../../../../../code/crosswords/business/internal/positioner003.h \
    ../../../../../code/crosswords/business/internal/positioner004.h

LIBS += -ltenacitas.crosswords.entities

