QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.entities.crosswords


HEADERS += \
    ../../../../../code/entities/crosswords/board.h \
    ../../../../../code/entities/crosswords/coordinate.h \
    ../../../../../code/entities/crosswords/description.h \
    ../../../../../code/entities/crosswords/dictionary.h \
    ../../../../../code/entities/crosswords/item.h \
    ../../../../../code/entities/crosswords/lexeme.h \
    ../../../../../code/entities/crosswords/limit.h \
    ../../../../../code/entities/crosswords/word.h \
    ../../../../../code/entities/crosswords/words.h \
    ../../../../../code/type/entities/number.h

include(../../../common.pri)


