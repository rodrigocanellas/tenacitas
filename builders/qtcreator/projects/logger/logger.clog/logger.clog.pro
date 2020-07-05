CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.clog
include (../../../common.pri)

HEADERS += \
  $$code_src_dir/logger/internal/log_t.h \
  $$code_src_dir/logger/clog/log.h

SOURCES += \
  $$code_src_dir/logger/clog/internal/log.cpp


