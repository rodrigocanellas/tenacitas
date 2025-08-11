QT += core widgets
TEMPLATE = app
TARGET = tnct.async.exp.temperature_sensors_simulator
CONFIG += example
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/async/exp/temperature_sensors_simulator

SOURCES += \
    $$PRJ_DIR/uix/qt/main.cpp \
    $$PRJ_DIR/uix/qt/mainwindow.cpp

FORMS += \
    $$PRJ_DIR/uix/qt/mainwindow.ui

HEADERS += \
    $$PRJ_DIR/uix/qt/mainwindow.h

HEADERS += \
    $$PRJ_DIR/typ/sensor_id.h \
    $$PRJ_DIR/typ/temperature.h

HEADERS += \
    $$PRJ_DIR/evt/add_sensor.h \
    $$PRJ_DIR/evt/new_temperature.h \
    $$PRJ_DIR/evt/remove_sensor.h \
    $$PRJ_DIR/evt/set_temperature.h

HEADERS += \
    $$PRJ_DIR/per/sensor.h \
    $$PRJ_DIR/per/sensors.h \

HEADERS += \
    $$PRJ_DIR/dat/sensor_id.h \
    $$PRJ_DIR/dat/temperature.h


HEADERS += \
    $$PRJ_DIR/supplier.h

