CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.cerr
include (../../../common.pri)


HEADERS += \
    $$code_src_dir/logger/internal/level.h \
    $$code_src_dir/logger/internal/log_t.h \
    $$code_src_dir/logger/cerr/log.h

SOURCES += \
  $$code_src_dir/logger/cerr/internal/log.cpp




