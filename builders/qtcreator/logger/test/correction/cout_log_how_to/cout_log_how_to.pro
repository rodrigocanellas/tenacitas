


TEMPLATE = app
TARGET = tenacitas.logger.test.correction.cout_log_how_to
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui




message("LIBS = $$LIBS")

SOURCES += \
	    ../../../../../../code/test/logger/correction/cout_log_how_to/main.cpp


LIBS += -ltenacitas.business.logger


include (../../../../common.pri)
