TEMPLATE=aux


INCLUDEPATH=../../..

CONFIG += c++20

QMAKE_CXXFLAGS += -std=c++20

HEADERS = \
	../../async/alg/dispatcher.h \
	../../async/alg/dispatcher.h \
	../../async/alg/execute.h \
	../../async/alg/sleeping_loop.h \
	../../async/cpt/concepts.h \
	../../async/internal/typ/queue.h \
	../../async/typ/priority.h \
	../../async/typ/queue_id.h \
	../../async/typ/subscriber.h \
	../../calendar/cpt/chrono_convertible.h \
	../../communication/alg/channel.h \
	../../communication/alg/message.h \
	../../communication/cpt/concepts.h \
	../../communication/evt/connection_closed.h \
	../../communication/evt/new_connection.h \
	../../communication/evt/send_timeout.h \
	../../communication/internal/alg/in_channel.h \
	../../communication/internal/alg/out_channel.h \
	../../communication/internal/evt/deserialize.h \
	../../communication/internal/evt/new_message.h \
	../../communication/internal/evt/send_msg.h \
	../../communication/typ/channel_id.h \
	../../communication/typ/connection_id.h \
	../../container/typ/circular_queue.h \
	../../container/typ/matrix.h \
	../../conversions/alg/cvt.h \
	../../conversions/alg/int.h \
	../../conversions/alg/internal/internal.h \
	../../conversions/typ/base.h \
	../../conversions/typ/result.h \
	../../conversions/typ/separator.h \
	../../crosswords/alg/assembler.h \
	../../crosswords/evt/events.h \
	../../crosswords/typ/grid.h \
	../../ipc/alg/active_connector.h \
	../../ipc/alg/connection.h \
	../../ipc/alg/messenger.OLD.h \
	../../ipc/alg/passive_connector.h \
	../../ipc/alg/posix/active_connector.h \
	../../ipc/alg/posix/connection.h \
	../../ipc/alg/posix/passive_connector.h \
	../../ipc/alg/posix/server.h \
	../../ipc/alg/posix/timeout.h \
	../../ipc/alg/receiver.h \
	../../ipc/alg/sender.h \
	../../ipc/alg/timeout.h \
	../../ipc/evt/events.h \
	../../ipc/typ/types.h \
	../../log/alg/internal/file.h \
	../../log/alg/internal/wrapper.h \
	../../log/alg/logger.h \
	../../log/typ/level.h \
	../../math/alg/factorial.h \
	../../number/alg/format.h \
	../../number/cpt/id.h \
	../../number/typ/id.h \
	../../number/typ/uuid.h \
	../../program/alg/options.h \
	../../test/alg/tester.h \
	../../traits/alg/pair.h \
	../../traits/alg/pointer.h \
	../../traits/alg/traits.h \
	../../traits/alg/tuple.h \
	../../traits/cpt/is_tuple.h
