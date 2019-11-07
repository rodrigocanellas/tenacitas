QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.string.business


HEADERS += \
    ../../../../../code/string/business/file_name_from_path.h \
    ../../../../../code/string/business/format_number.h \
    ../../../../../code/string/business/max_str_length.h

SOURCES += \
    ../../../../../code/string/business/file_name_from_path.cpp

include (../../../common.pri)
