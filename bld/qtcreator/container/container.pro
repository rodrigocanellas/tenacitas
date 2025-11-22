include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst \
          exp


HEADERS += \
        $$BASE_DIR/tnct/container/cpt/queue.h \
        $$BASE_DIR/tnct/container/cpt/matrix.h \
        $$BASE_DIR/tnct/container/dat/circular_queue.h \
        $$BASE_DIR/tnct/container/dat/circular_queue_with_notifier.h \
        $$BASE_DIR/tnct/container/dat/matrix.h \
        $$BASE_DIR/tnct/container/bus/multiply_matrix.h \
        $$BASE_DIR/tnct/container/internal/bus/multiply_matrix_async.h \
        $$BASE_DIR/tnct/container/internal/evt/multiply_matrix_cell.h \
        $$BASE_DIR/tnct/container/internal/bus/create_matrix_for_multiply.h



