QT += core widgets
TEMPLATE = app
TARGET = tnct.async.exp.temperature_sensors_simulator
CONFIG += example
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/async/exp

SOURCES += \
    $$PRJ_DIR/temperature_sensors_simulator/uix/qt/main.cpp \
    $$PRJ_DIR/temperature_sensors_simulator/uix/qt/mainwindow.cpp

FORMS += \
    $$PRJ_DIR/temperature_sensors_simulator/uix/qt/mainwindow.ui

HEADERS += \
    $$PRJ_DIR/temperature_sensors_simulator/uix/qt/mainwindow.h

HEADERS += \
    $$PRJ_DIR/temperature_sensors_simulator/typ/sensor_id.h \
    $$PRJ_DIR/temperature_sensors_simulator/typ/temperature.h

HEADERS += \
    $$PRJ_DIR/temperature_sensors_simulator/evt/add_sensor.h \
    $$PRJ_DIR/temperature_sensors_simulator/evt/new_temperature.h \
    $$PRJ_DIR/temperature_sensors_simulator/evt/remove_sensor.h \
    $$PRJ_DIR/temperature_sensors_simulator/evt/set_temperature.h

HEADERS += \
    $$PRJ_DIR/temperature_sensors_simulator/per/sensor.h \
    $$PRJ_DIR/temperature_sensors_simulator/per/sensors.h

HEADERS += \
    $$PRJ_DIR/temperature_sensors_simulator/supplier.h

