include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/log

HEADERS += \
        $$PRJ_DIR/cout.h \
        $$PRJ_DIR/file.h \
        $$PRJ_DIR/cerr.h \
        $$PRJ_DIR/level.h \
        $$PRJ_DIR/int/logger.h \
        $$PRJ_DIR/no_logger.h  \
        $$PRJ_DIR/logger.h \
        $$PRJ_DIR/line_header_formater.h \
        $$PRJ_DIR/traits/writer.h \
        $$PRJ_DIR/traits/logger.h \
