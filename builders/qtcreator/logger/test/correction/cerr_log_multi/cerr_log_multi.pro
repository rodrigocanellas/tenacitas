


TEMPLATE = app
TARGET = tenacitas.logger.test.correction.cerr_log_multi
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui




message("LIBS = $$LIBS")

SOURCES += \
    ../../../../../../code/logger/test/correction/cerr_log_multi/main.cpp


include (../../../../common.pri)



LIBS += -ltenacitas.logger.business
