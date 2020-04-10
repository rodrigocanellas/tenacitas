CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.cerr
include (../../../common.pri)


HEADERS += \
    $$code_src_dir/logger/_bus/level.h \
    $$code_src_dir/logger/_bus/log_t.h \
    $$code_src_dir/logger/cerr/log.h

SOURCES += \
  $$code_src_dir/logger/cerr/internal/log.cpp




