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
    ../../../../code/interpreter/business/internal/recognizers.h \
    ../../../../code/interpreter/business/internal/scanner.h \
    ../../../../code/interpreter/business/internal/symbol.h \
    ../../../../code/interpreter/business/internal/symbols.h \
    ../../../../code/interpreter/business/internal/tokens.h \
    ../../../../code/interpreter/business/type.h \
    ../../../../code/interpreter/business/recognizer.h \
    ../../../../code/string/business/split_str.h

SOURCES += \
    ../../../../code/interpreter/business/internal/scanner.cpp \
    ../../../../code/interpreter/business/internal/symbol.cpp \
    ../../../../code/interpreter/business/internal/tokens.cpp \
    ../../../../code/interpreter/business/internal/type.cpp


LIBS+=-ltenacitas.string

DISTFILES += \
    WhereDidIStop.txt
