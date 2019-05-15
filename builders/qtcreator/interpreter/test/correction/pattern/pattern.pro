
TEMPLATE = app
TARGET = tenacitas.interpreter.test.correction.type
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../common.pri)

message("LIBS = $$LIBS")


LIBS += -ltenacitas.interpreter -ltenacitas.logger -ltenacitas.string

SOURCES += \
    ../../../../../../code/interpreter/test/correction/pattern/main.cpp

