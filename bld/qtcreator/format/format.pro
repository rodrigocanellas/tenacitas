include (../common.pri)


TEMPLATE=subdirs

SUBDIRS = tst

HEADERS += \
        $$BASE_DIR/tnct/format/format_number.h \
        $$BASE_DIR/tnct/format/align.h \
        $$BASE_DIR/tnct/format/fmt.h \
        $$BASE_DIR/tnct/format/max_str_length.h



