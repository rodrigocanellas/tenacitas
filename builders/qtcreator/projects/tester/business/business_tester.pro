
QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.business.tester


HEADERS += \
    ../../../../../code/business/tester/internal/test.h \
    ../../../../../code/business/tester/run.h

include (../../../common.pri)


LIBS += -ltenacitas.business.logger
