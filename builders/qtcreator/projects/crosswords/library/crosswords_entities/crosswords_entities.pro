QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.crosswords.entities


HEADERS += \
    ../../../../../../code/crosswords/entities/board.h \
    ../../../../../../code/crosswords/entities/coordinate.h \
    ../../../../../../code/crosswords/entities/description.h \
    ../../../../../../code/crosswords/entities/dictionary.h \
    ../../../../../../code/crosswords/entities/item.h \
    ../../../../../../code/crosswords/entities/lexeme.h \
    ../../../../../../code/crosswords/entities/limit.h \
    ../../../../../../code/crosswords/entities/word.h \
    ../../../../../../code/crosswords/entities/words.h \
    ../../../../../../code/type/entities/number.h

include(../../../../common.pri)


