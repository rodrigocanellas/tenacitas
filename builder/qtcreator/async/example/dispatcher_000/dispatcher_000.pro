QT -= core
TEMPLATE = app
TARGET = tenacitas.lib.async.exp.dispatcher_000
CONFIG += example
include (../../../common.pri)

HEADERS+=$$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/publisher.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/logger.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/event.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/handler.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/event_handled.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/event_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/handler_type_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/handler_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/handling_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/results.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/configuration.h


SOURCES = $$BASE_DIR/tenacitas.lib/async/exp/dispatcher_000/main.cpp

DISTFILES += \
    ../../../../../async/exp/dispatcher_000/cfg.ini


