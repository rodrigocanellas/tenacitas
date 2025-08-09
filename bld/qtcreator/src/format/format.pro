include (../../common.pri)


TEMPLATE=subdirs

SUBDIRS = test

HEADERS += \
        $$BASE_DIR/tenacitas/src/format/format_number.h \
        $$BASE_DIR/tenacitas/src/format/align.h \
        $$BASE_DIR/tenacitas/src/format/fmt.h \
        $$BASE_DIR/tenacitas/src/format/max_str_length.h



