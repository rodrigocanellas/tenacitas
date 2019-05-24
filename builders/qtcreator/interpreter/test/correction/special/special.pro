
TEMPLATE = app
TARGET = tenacitas.interpreter.test.correction.special
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../common.pri)

message("LIBS = $$LIBS")


LIBS += -ltenacitas.interpreter -ltenacitas.logger -ltenacitas.string

SOURCES += \
    ../../../../../../code/interpreter/test/correction/special/main.cpp



