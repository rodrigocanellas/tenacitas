include (../../common.pri)


TEMPLATE=aux

PRJ_DIR=$$BASE_DIR/tnct/crosswords/evt


HEADERS +=  \
    $$PRJ_DIR/internal/grid_permutations_tried.h  \
    $$PRJ_DIR/internal/grid_create_stop.h \
    $$PRJ_DIR/internal/grid_create_start.h \
    $$PRJ_DIR/internal/grid_create_solved.h \
    $$PRJ_DIR/internal/grid_create_unsolved.h \
    $$PRJ_DIR/internal/grid_to_organize.h \
    $$PRJ_DIR/internal/grid_attempt_configuration.h
