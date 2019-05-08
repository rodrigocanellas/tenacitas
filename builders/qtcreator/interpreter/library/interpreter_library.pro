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
    ../../../../code/interpreter/business/internal/scanner.h \
    ../../../../code/interpreter/business/internal/symbol.h \
    ../../../../code/interpreter/business/internal/symbols.h \
    ../../../../code/interpreter/business/internal/type.h \
    ../../../../code/interpreter/business/internal/types.h \
    ../../../../code/string/business/split_str.h

SOURCES += \
    ../../../../code/interpreter/business/internal/scanner.cpp \
    ../../../../code/interpreter/business/internal/symbol.cpp \
    ../../../../code/interpreter/business/internal/type.cpp \
    ../../../../code/interpreter/business/internal/types.cpp


LIBS+=-ltenacitas.string
