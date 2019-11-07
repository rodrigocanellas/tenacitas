
QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.tester.business


HEADERS += \
    ../../../../../code/tester/business/internal/test.h \
    ../../../../../code/tester/business/run.h

include (../../../common.pri)


LIBS += -ltenacitas.logger.business -ltenacitas.string.business -ltenacitas.string.business -ltenacitas.string.business
