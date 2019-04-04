QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.interpreter


include (../../common.pri)

LIBS+=-ltenacitas.logger

HEADERS += \
    ../../../../code/interpreter/business/internal/lexeme.h \
    ../../../../code/interpreter/business/internal/lexeme.h \
    ../../../../code/interpreter/business/internal/str2lcol.h \
    ../../../../code/interpreter/business/internal/symbol.h \
    ../../../../code/interpreter/business/internal/type.h

SOURCES += \
    ../../../../code/interpreter/business/internal/symbol.cpp


