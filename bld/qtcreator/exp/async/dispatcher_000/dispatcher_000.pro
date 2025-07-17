QT -= core
TEMPLATE = app
TARGET = tnct.exp.async.dispatcher_000
CONFIG += example
include (../../../common.pri)


PRJ_DIR=$$BASE_DIR_EXP/tnct/async/

HEADERS+=$$PRJ_DIR/dispatcher_000/publisher.h \
         $$PRJ_DIR/dispatcher_000/logger.h \
         $$PRJ_DIR/dispatcher_000/event.h \
         $$PRJ_DIR/dispatcher_000/handler.h \
         $$PRJ_DIR/dispatcher_000/event_handled.h \
         $$PRJ_DIR/dispatcher_000/event_id.h \
         $$PRJ_DIR/dispatcher_000/handler_type_id.h \
         $$PRJ_DIR/dispatcher_000/results.h \
         $$PRJ_DIR/dispatcher_000/configuration.h


SOURCES = $$PRJ_DIR/dispatcher_000/main.cpp

DISTFILES += \
    $$PRJ_DIR/dispatcher_000/cfg.ini \
    ../../../../../src/tnct/async/exp/dispatcher_000/cfg-a.ini \
    ../../../../../src/tnct/async/exp/dispatcher_000/cfg-b.ini \
    ../../../../../src/tnct/async/exp/dispatcher_000/cfg-c.ini \
    ../../../../../src/tnct/async/exp/dispatcher_000/cfg-d.ini \
    ../../../../../src/tnct/async/exp/dispatcher_000/cfg-e.ini \
    ../../../../../src/tnct/async/exp/dispatcher_000/cfg-f.ini
