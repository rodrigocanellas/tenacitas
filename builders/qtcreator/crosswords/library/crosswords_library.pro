QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.crosswords


HEADERS += \  \ \
    ../../../../code/crosswords/business/internal/log.h \
    ../../../../code/crosswords/business/positioner000.h \
    ../../../../code/crosswords/entities/board.h \
    ../../../../code/crosswords/entities/description.h \
    ../../../../code/crosswords/entities/dictionary.h \
    ../../../../code/crosswords/entities/item.h \
    ../../../../code/crosswords/entities/limit.h \
    ../../../../code/crosswords/entities/position.h \
    ../../../../code/crosswords/entities/positions.h \
    ../../../../code/crosswords/entities/word.h


SOURCES +=


include (../../common.pri)




