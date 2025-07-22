include (../../../common.pri)


TEMPLATE=lib

HEADERS +=  \
    $$BASE_DIR/tenacitas/src/crosswords/asy/events.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_permutations_tried.h  \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_create_stop.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_create_start.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_create_solved.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_create_unsolved.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_create_timeout.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_to_organize.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_create_status.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/grid_attempt_configuration.h \
    $$BASE_DIR/tenacitas/src/crosswords/asy/dispatcher.h
