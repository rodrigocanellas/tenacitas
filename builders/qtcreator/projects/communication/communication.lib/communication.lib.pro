CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.communication


include (../../../common.pri)


HEADERS += \
    $$code_src_dir/communication/client.h \
    $$code_src_dir/communication/internal/log.h \
    ../../../../../doc/projects/communication/todo.h \
    ../../../../../src/communication/internal/timeout.h \
    ../../../../../src/communication/status.h

SOURCES += \
  ../../../../../src/communication/internal/log.cpp \
  ../../../../../src/communication/internal/status.cpp
