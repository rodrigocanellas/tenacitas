include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


HEADERS += \
        $$BASE_DIR/tnct/container/dat/circular_queue.h \
        $$BASE_DIR/tnct/container/dat/circular_queue_with_notifier.h \
        $$BASE_DIR/tnct/container/cpt/queue.h \
        $$BASE_DIR/tnct/container/dat/matrix.h



