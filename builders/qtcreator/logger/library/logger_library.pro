QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.logger


HEADERS += \
    ../../../../code/logger/bus/cerr.h \
    ../../../../code/logger/bus/file.h \
    ../../../../code/logger/bus/cout.h \
    ../../../../code/logger/bus/internal/level.h \
    ../../../../code/logger/bus/internal/log.h \
    ../../../../code/logger/bus/internal/configure_file_log.h \
    ../../../../code/logger/bus/internal/configure_cerr_log.h \
    ../../../../code/logger/bus/internal/configure_cout_log.h \
    ../../../../code/logger/bus/internal/file_controller.h \
    ../../../../code/logger/bus/internal/cerr_media.h \
    ../../../../code/logger/bus/internal/file_media.h \
    ../../../../code/logger/bus/internal/cout_media.h \
    ../../../../code/calendar/bus/conversions.h \
    ../../../../code/calendar/bus/epoch.h


SOURCES += \
    ../../../../code/logger/bus/internal/file.cpp \
    ../../../../code/logger/bus/internal/file_controller.cpp \
    ../../../../code/logger/bus/internal/cerr.cpp \
    ../../../../code/logger/bus/internal/cout.cpp


include (../../common.pri)

#LIBS+=-ltenacitas.concurrent

message("LIBS = "$$LIBS)



