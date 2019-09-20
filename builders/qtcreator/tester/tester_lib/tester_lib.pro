
QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.tester



include (../../common.pri)

HEADERS += \
    ../../../../code/tester/business/internal/test.h \
    ../../../../code/tester/business/run.h

LIBS += -ltenacitas.logger
