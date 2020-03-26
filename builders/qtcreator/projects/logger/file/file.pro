CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

TARGET = tenacitas.logger.file

include (../../../../builders/qtcreator/common.pri)

HEADERS += \
    ../../../file/file.h \
    ../../../file/internal/file_controller.h \
    ../../../file/internal/file_writer.h

SOURCES += \
    ../../../file/internal/file_controller.cpp


