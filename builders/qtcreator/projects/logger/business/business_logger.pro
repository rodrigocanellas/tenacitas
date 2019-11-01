QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.business.logger


HEADERS += \
    ../../../../../code/business/logger/log.h \
    ../../../../../code/business/logger/cerr.h \
    ../../../../../code/business/logger/file.h \
    ../../../../../code/business/logger/cout.h \
    ../../../../../code/business/logger/internal/file_writer.h \
    ../../../../../code/business/logger/internal/level.h \
    ../../../../../code/business/logger/internal/file_controller.h \
    ../../../../../../code/calendar/business/conversions.h \
    ../../../../../../code/calendar/business/epoch.h


SOURCES += \
    ../../../../../code/business/logger/internal/file_controller.cpp \
    ../../../../../code/business/logger/internal/log.cpp




LIBS+=-lpthread

include (../../../common.pri)

message("LIBS = "$$LIBS)



