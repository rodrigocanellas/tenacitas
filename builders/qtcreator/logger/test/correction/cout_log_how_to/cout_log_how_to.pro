


TEMPLATE = app
TARGET = tenacitas.logger.test.correction.cout_log_how_to
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui




message("LIBS = $$LIBS")

SOURCES += \
	    ../../../../../../code/logger/test/correction/cout_log_how_to/main.cpp


include (../../../../common.pri)



LIBS += -ltenacitas.logger
