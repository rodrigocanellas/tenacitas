include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS = test


HEADERS += \
        $$BASE_DIR/tenacitas/src/container/matrix.h \
        $$BASE_DIR/tenacitas/src/container/circular_queue.h \
        $$BASE_DIR/tenacitas/src/container/traits/queue.h



