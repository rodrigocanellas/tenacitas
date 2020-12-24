CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.clog
include (../../../common.pri)

HEADERS += \
  $$code_src_dir/logger/internal/log.h \
  $$code_src_dir/logger/clog.h \
  $$code_src_dir/logger/level.h


SOURCES += \
  $$code_src_dir/logger/internal/log.cpp  \
  $$code_src_dir/logger/internal/level.cpp


