CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.cout
include (../../../_common.pri)

HEADERS += \
  $$code_src_dir/logger/_common/level.h \
  $$code_src_dir/logger/_common/log_t.h \
  $$code_src_dir/logger/cout/log.h

SOURCES += \
  $$code_src_dir/logger/cout/internal/log.cpp
