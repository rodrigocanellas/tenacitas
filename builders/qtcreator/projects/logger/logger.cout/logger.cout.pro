CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.cout
include (../../../common.pri)

HEADERS += \
  $$code_src_dir/logger/internal/log.h \
  $$code_src_dir/logger/cout.h \
  $$code_src_dir/logger/level.h

SOURCES += \
  $$code_src_dir/logger/internal/log.cpp \
  $$code_src_dir/logger/internal/level.cpp

