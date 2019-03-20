


TEMPLATE = app
TARGET = tenacitas.logger.test.correction.file_log_creation
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui

include (../../../../common.pri)



message("LIBS = $$LIBS")

SOURCES += \
    ../../../../../../code/logger/test/correction/file_log_creation/main.cpp


LIBS += -ltenacitas.logger

