QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.logger.business


HEADERS += \
    ../../../../../code/logger/business/log.h \
    ../../../../../code/logger/business/cerr.h \
    ../../../../../code/logger/business/file.h \
    ../../../../../code/logger/business/cout.h \
    ../../../../../code/logger/business/internal/file_writer.h \
    ../../../../../code/logger/business/internal/level.h \
    ../../../../../code/logger/business/internal/file_controller.h \
    ../../../../../code/calendar/business/conversions.h \
    ../../../../../code/calendar/business/epoch.h


SOURCES += \
    ../../../../../code/logger/business/internal/file_controller.cpp \
    ../../../../../code/logger/business/internal/log.cpp


include (../../../common.pri)

LIBS += -lpthread
LIBS += -ltenacitas.string.business



message("LIBS = "$$LIBS)



