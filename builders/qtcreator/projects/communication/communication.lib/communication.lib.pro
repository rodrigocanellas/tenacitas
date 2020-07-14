CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.communication


include (../../../common.pri)


HEADERS += \
    $$code_src_dir/communication/client.h \
    $$code_src_dir/communication/internal/log.h \
    $$code_src_dir/communication/internal/timeout.h \
    $$code_src_dir/communication/status.h

SOURCES += \
  $$code_src_dir/communication/internal/log.cpp \
  $$code_src_dir/communication/internal/status.cpp
