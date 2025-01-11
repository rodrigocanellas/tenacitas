QT += core widgets
TEMPLATE = app
TARGET = tenacitas.lib.async.exp.events_simulation
CONFIG += example
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas.lib/async/exp/events_simulation

SOURCES += \
    $$PRJ_DIR/itf/main.cpp \
    $$PRJ_DIR/itf/main_window.cpp

HEADERS += \
    $$PRJ_DIR/itf/main_window.h

HEADERS += \
    $$PRJ_DIR/cfg/handlings.h \
    $$PRJ_DIR/cfg/publishers.h \
    $$PRJ_DIR/cfg/reader.h

HEADERS += \
    $$PRJ_DIR/eve/dispatcher.h \
    $$PRJ_DIR/eve/configuration_defined.h \
    $$PRJ_DIR/eve/simulation_finished.h \
    $$PRJ_DIR/eve/simulation_a.h \
    $$PRJ_DIR/eve/simulation_b.h \
    $$PRJ_DIR/eve/simulation_c.h

HEADERS += \
    $$PRJ_DIR/bus/simulation_handler.h \
    $$PRJ_DIR/bus/configuration_defined_handler.h

HEADERS += \
    $$PRJ_DIR/typ/handler_id.h \
    $$PRJ_DIR/typ/handling_id.h \
    $$PRJ_DIR/typ/publisher_id.h

FORMS += \
    $$PRJ_DIR/itf/main_window.ui

DISTFILES += \
    $$PRJ_DIR/cfg/cfg_000.ini


