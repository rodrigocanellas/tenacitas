CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.cout
include (../../../common.pri)

HEADERS += \
  $$code_src_dir/logger/internal/log_t.h \
  $$code_src_dir/logger/cout/log.h

SOURCES += \
  $$code_src_dir/logger/cout/internal/log.cpp
