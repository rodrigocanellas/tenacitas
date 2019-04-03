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
    ../../../../code/interpreter/business/symbol.h \
    ../../../../code/interpreter/business/type.h \
    ../../../../code/interpreter/business/value.h


