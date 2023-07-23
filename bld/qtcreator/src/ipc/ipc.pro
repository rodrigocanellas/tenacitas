TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$SRC_DIR/tenacitas/ipc/alg/active_connector.h \
        $$SRC_DIR/tenacitas/ipc/alg/connection.h \
        $$SRC_DIR/tenacitas/ipc/alg/passive_connector.h \
        $$SRC_DIR/tenacitas/ipc/alg/posix/active_connector.h \
        $$SRC_DIR/tenacitas/ipc/alg/posix/connection.h \
        $$SRC_DIR/tenacitas/ipc/alg/posix/passive_connector.h \
        $$SRC_DIR/tenacitas/ipc/alg/posix/server.h \
        $$SRC_DIR/tenacitas/ipc/alg/posix/timeout.h \
        $$SRC_DIR/tenacitas/ipc/alg/receiver.h \
        $$SRC_DIR/tenacitas/ipc/alg/sender.h \
        $$SRC_DIR/tenacitas/ipc/alg/timeout.h \
        $$SRC_DIR/tenacitas/ipc/evt/events.h \
        $$SRC_DIR/tenacitas/ipc/typ/types.h \


