include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
        $$BASE_DIR/tenacitas.lib/log/cout.h \
        $$BASE_DIR/tenacitas.lib/log/file.h \
        $$BASE_DIR/tenacitas.lib/log/cerr.h \
        $$BASE_DIR/tenacitas.lib/log/internal/level.h \
        $$BASE_DIR/tenacitas.lib/log/internal/logger.h \
        $$BASE_DIR/tenacitas.lib/log/no_logger.h  \
        $$BASE_DIR/tenacitas.lib/log/logger.h



