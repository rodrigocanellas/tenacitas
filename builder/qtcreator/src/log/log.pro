include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
        $$BASE_SRC_DIR/tnct/log/cout.h \
        $$BASE_SRC_DIR/tnct/log/file.h \
        $$BASE_SRC_DIR/tnct/log/cerr.h \
        $$BASE_SRC_DIR/tnct/log/level.h \
        $$BASE_SRC_DIR/tnct/log/internal/logger.h \
        $$BASE_SRC_DIR/tnct/log/no_logger.h  \
        $$BASE_SRC_DIR/tnct/log/logger.h \
        $$BASE_SRC_DIR/tnct/log/line_header_formater.h



