CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.file
include (../../../common.pri)

HEADERS += \
    $$code_src_dir/logger/file/internal/file_controller.h \
    $$code_src_dir/logger/file/internal/file_writer.h \
    $$code_src_dir/logger/file/log.h

SOURCES += \
    $$code_src_dir/logger/file/internal/file_controller.cpp \
    $$code_src_dir/logger/file/internal/log.cpp


LIBS+=$$libs_dir/libconcurrent.lib.$$static_lib_ext
