TEMPLATE = app
TARGET = tenacitas.interpreter.test.correction.token
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../common.pri)



message("LIBS = $$LIBS")


LIBS += -ltenacitas.logger -ltenacitas.string -ltenacitas.interpreter

SOURCES += \
    ../../../../../../code/interpreter/test/correction/token/main.cpp

