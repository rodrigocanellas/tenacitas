include (../../common.pri)


TEMPLATE=subdirs

SUBDIRS = test


HEADERS += \
    $$BASE_DIR/tenacitas/src/time/output.h \
    $$BASE_DIR/tenacitas/src/time/traits/chrono_convertible.h

