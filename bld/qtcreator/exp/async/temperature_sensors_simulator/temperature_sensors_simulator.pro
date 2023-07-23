QT += core widgets
TEMPLATE = app
TARGET = tenacitas.lib.exp.async.temperature_sensors_simulator
CONFIG += example
include (../../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES += \
    $$EXP_DIR/async/temperature_sensors_simulator/uix/qt/main.cpp \
    $$EXP_DIR/async/temperature_sensors_simulator/uix/qt/mainwindow.cpp

FORMS += \
    $$EXP_DIR/async/temperature_sensors_simulator/uix/qt/mainwindow.ui

HEADERS += \
    $$EXP_DIR/async/temperature_sensors_simulator/uix/qt/mainwindow.h

HEADERS += \
    $$EXP_DIR/async/temperature_sensors_simulator/dat/sensor_id.h \
    $$EXP_DIR/async/temperature_sensors_simulator/dat/temperature.h

HEADERS += \
    $$EXP_DIR/async/temperature_sensors_simulator/evt/add_sensor.h \
    $$EXP_DIR/async/temperature_sensors_simulator/evt/new_temperature.h \
    $$EXP_DIR/async/temperature_sensors_simulator/evt/remove_sensor.h \
    $$EXP_DIR/async/temperature_sensors_simulator/evt/set_temperature.h

HEADERS += \
    $$EXP_DIR/async/temperature_sensors_simulator/per/sensor.h \
    $$EXP_DIR/async/temperature_sensors_simulator/per/sensors.h

HEADERS += \
    $$EXP_DIR/async/temperature_sensors_simulator/alg/dispatcher.h

