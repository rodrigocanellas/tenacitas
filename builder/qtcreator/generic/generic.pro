include (../common.pri)


TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
        $$BASE_DIR/tenacitas.lib/generic/tester.h \
        $$BASE_DIR/tenacitas.lib/generic/format_number.h \
        $$BASE_DIR/tenacitas.lib/generic/id.h \
        $$BASE_DIR/tenacitas.lib/generic/align.h \
        $$BASE_DIR/tenacitas.lib/generic/internal/max_str_length.h \
        $$BASE_DIR/tenacitas.lib/generic/chrono_output.h \
        $$BASE_DIR/tenacitas.lib/generic/tuple_output.h \
        $$BASE_DIR/tenacitas.lib/generic/fmt.h \
        $$BASE_DIR/tenacitas.lib/generic/fixed_size_string.h \
        $$BASE_DIR/tenacitas.lib/generic/logger.h



