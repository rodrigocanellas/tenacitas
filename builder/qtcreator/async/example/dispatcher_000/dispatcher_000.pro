QT -= core
TEMPLATE = app
TARGET = tnct.async.exp.dispatcher_000
CONFIG += example
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/async/exp

HEADERS+=$$PRJ_DIR/dispatcher_000/publisher.h \
         $$PRJ_DIR/dispatcher_000/logger.h \
         $$PRJ_DIR/dispatcher_000/event.h \
         $$PRJ_DIR/dispatcher_000/handler.h \
         $$PRJ_DIR/dispatcher_000/event_handled.h \
         $$PRJ_DIR/dispatcher_000/event_id.h \
         $$PRJ_DIR/dispatcher_000/handler_type_id.h \
         $$PRJ_DIR/dispatcher_000/handler_id.h \
         $$PRJ_DIR/dispatcher_000/handling_id.h \
         $$PRJ_DIR/dispatcher_000/results.h \
         $$PRJ_DIR/dispatcher_000/configuration.h


SOURCES = $$PRJ_DIR/dispatcher_000/main.cpp

DISTFILES += \
    $$PRJ_DIR/dispatcher_000/cfg.ini


