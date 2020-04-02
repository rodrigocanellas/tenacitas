CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.file

include (../../../common.pri)

HEADERS += \
    ../../../../../code/logger/file/internal/file_controller.h \
    ../../../../../code/logger/file/internal/file_writer.h \
    ../../../../../code/logger/file/log.h

SOURCES += \
    ../../../../../code/logger/file/internal/file_controller.cpp \
    ../../../../../code/logger/file/internal/log.cpp


