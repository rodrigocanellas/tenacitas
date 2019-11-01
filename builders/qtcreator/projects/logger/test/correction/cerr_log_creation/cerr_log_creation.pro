


TEMPLATE = app
TARGET = tenacitas.logger.test.correction.cerr_log_creation
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../../common.pri)



message("LIBS = $$LIBS")

SOURCES += \
    ../../../../../../../code/test/logger/correction/cerr_log_creation/main.cpp




LIBS +=-ltenacitas.business.logger
