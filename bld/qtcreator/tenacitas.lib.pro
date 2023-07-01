TEMPLATE=aux

include (../../../tenacitas.bld/qtcreator/common.pri)

#INCLUDEPATH=../../..

CONFIG += c++20

QMAKE_CXXFLAGS += -std=c++20

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/async/alg/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/src/async/alg/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/src/async/alg/execute.h \
        $$BASE_DIR/tenacitas.lib/src/async/alg/sleeping_loop.h \
        $$BASE_DIR/tenacitas.lib/src/async/cpt/concepts.h \
        $$BASE_DIR/tenacitas.lib/src/async/internal/typ/queue.h \
        $$BASE_DIR/tenacitas.lib/src/async/typ/priority.h \
        $$BASE_DIR/tenacitas.lib/src/async/typ/queue_id.h \
        $$BASE_DIR/tenacitas.lib/src/async/typ/subscriber.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/calendar/cpt/chrono_convertible.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/message.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/concepts.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/connection_closed.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/new_connection.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/send_timeout.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/alg/in_channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/alg/out_channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/deserialize.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/new_message.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/send_msg.h \
        $$BASE_DIR/tenacitas.lib/src/communication/typ/channel_id.h \
        $$BASE_DIR/tenacitas.lib/src/communication/typ/connection_id.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/container/typ/circular_queue.h \
        $$BASE_DIR/tenacitas.lib/src/container/typ/matrix.h \
        $$BASE_DIR/tenacitas.lib/src/conversions/alg/cvt.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/conversions/alg/int.h \
        $$BASE_DIR/tenacitas.lib/src/conversions/alg/internal/internal.h \
        $$BASE_DIR/tenacitas.lib/src/conversions/typ/base.h \
        $$BASE_DIR/tenacitas.lib/src/conversions/typ/result.h \
        $$BASE_DIR/tenacitas.lib/src/conversions/typ/separator.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/crosswords/alg/assembler.h \
        $$BASE_DIR/tenacitas.lib/src/crosswords/evt/events.h \
        $$BASE_DIR/tenacitas.lib/src/crosswords/typ/grid.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/active_connector.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/connection.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/passive_connector.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/posix/active_connector.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/posix/connection.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/posix/passive_connector.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/posix/server.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/posix/timeout.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/receiver.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/sender.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/alg/timeout.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/evt/events.h \
        $$BASE_DIR/tenacitas.lib/src/ipc/typ/types.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/log/alg/internal/file.h \
        $$BASE_DIR/tenacitas.lib/src/log/alg/internal/wrapper.h \
        $$BASE_DIR/tenacitas.lib/src/log/alg/logger.h \
        $$BASE_DIR/tenacitas.lib/src/log/typ/level.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/math/alg/factorial.h \
        $$BASE_DIR/tenacitas.lib/src/number/alg/format.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/number/cpt/id.h \
        $$BASE_DIR/tenacitas.lib/src/number/typ/id.h \
        $$BASE_DIR/tenacitas.lib/src/number/typ/uuid.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/program/alg/options.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/test/alg/tester.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/traits/alg/pair.h \
        $$BASE_DIR/tenacitas.lib/src/traits/alg/pointer.h \
        $$BASE_DIR/tenacitas.lib/src/traits/alg/traits.h \
        $$BASE_DIR/tenacitas.lib/src/traits/alg/tuple.h \
        $$BASE_DIR/tenacitas.lib/src/traits/cpt/is_tuple.h
