QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.string



include (../../common.pri)

HEADERS += \
    ../../../../code/string/business/format_number.h \
    ../../../../code/string/business/max_str_length.h \
    ../../../../code/string/business/split_str.h

SOURCES += \
    ../../../../code/string/business/split_str.cpp




