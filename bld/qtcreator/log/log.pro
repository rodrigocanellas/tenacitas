include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/log

HEADERS += \
        $$PRJ_DIR/bus/cout.h \
        $$PRJ_DIR/bus/file.h \
        $$PRJ_DIR/bus/cerr.h \
        $$PRJ_DIR/dat/level.h \
        $$PRJ_DIR/bus/line_header_formater.h \
        $$PRJ_DIR/internal/bus/logger.h \
        $$PRJ_DIR/bus/no_logger.h  \
        $$PRJ_DIR/cpt/writer.h \
        $$PRJ_DIR/cpt/logger.h \
        $$PRJ_DIR/cpt/macros.h \

