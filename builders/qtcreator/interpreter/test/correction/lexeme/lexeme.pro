


TEMPLATE = app
TARGET = tenacitas.interpreter.test.correction.lexeme
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../common.pri)



message("LIBS = $$LIBS")


LIBS += -ltenacitas.logger -ltenacitas.string

SOURCES += \
    ../../../../../../code/interpreter/test/correction/lexeme/main.cpp

