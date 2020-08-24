CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
TARGET = tenacitas.logger.file
include (../../../common.pri)

# LIBS+=$$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

HEADERS += \
    $$code_src_dir/logger/file/internal/file_controller.h \
    $$code_src_dir/logger/file/internal/file_writer.h \
    $$code_src_dir/logger/file/log.h \
    $$code_src_dir/logger/cerr/log.h

SOURCES += \
    $$code_src_dir/logger/cerr/internal/cerr_log.cpp \
    $$code_src_dir/logger/file/internal/file_controller.cpp \
    $$code_src_dir/logger/file/internal/file_log.cpp




