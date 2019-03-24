QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.logger


HEADERS += \
    ../../../../code/logger/business/cerr.h \
    ../../../../code/logger/business/file.h \
    ../../../../code/logger/business/cout.h \
    ../../../../code/logger/business/internal/level.h \
    ../../../../code/logger/business/internal/log.h \
    ../../../../code/logger/business/internal/configure_file_log.h \
    ../../../../code/logger/business/internal/configure_cerr_log.h \
    ../../../../code/logger/business/internal/configure_cout_log.h \
    ../../../../code/logger/business/internal/file_controller.h \
    ../../../../code/logger/business/internal/cerr_media.h \
    ../../../../code/logger/business/internal/file_media.h \
    ../../../../code/logger/business/internal/cout_media.h \
    ../../../../code/calendar/business/conversions.h \
    ../../../../code/calendar/business/epoch.h


SOURCES += \
    ../../../../code/logger/business/internal/file.cpp \
    ../../../../code/logger/business/internal/file_controller.cpp \
    ../../../../code/logger/business/internal/cerr.cpp \
    ../../../../code/logger/business/internal/cout.cpp


include (../../common.pri)

#LIBS+=-ltenacitas.concurrent

message("LIBS = "$$LIBS)



