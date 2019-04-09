


TEMPLATE = app
TARGET = tenacitas.interpreter.test.correction.symbol
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../common.pri)



message("LIBS = $$LIBS")


LIBS += -ltenacitas.logger -ltenacitas.string

SOURCES += \
    ../../../../../../code/interpreter/test/correction/symbol/main.cpp

