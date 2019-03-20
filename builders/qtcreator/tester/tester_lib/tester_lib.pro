
QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.tester



include (../../common.pri)

HEADERS += \
    ../../../../code/tester/run.h \
    ../../../../code/tester/inc/test.h

