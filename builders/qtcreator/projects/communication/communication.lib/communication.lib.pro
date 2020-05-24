CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.communication


include (../../../common.pri)


HEADERS += \
    $$code_src_dir/communication/client.h \
    $$code_src_dir/communication/internal/log.h \
    ../../../../../src/communication/status.h

SOURCES += \
  ../../../../../src/communication/internal/status.cpp
