QT += core widgets
TEMPLATE = app
TARGET = tenacitas.lib.exp.async.temperature_sensors_simulator
CONFIG += example
include (../../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/uix/qt/main.cpp \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/uix/qt/mainwindow.cpp

FORMS += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/uix/qt/mainwindow.ui

HEADERS += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/uix/qt/mainwindow.h

HEADERS += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/dat/sensor_id.h \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/dat/temperature.h

HEADERS += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/evt/add_sensor.h \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/evt/new_temperature.h \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/evt/remove_sensor.h \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/evt/set_temperature.h

HEADERS += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/per/sensor.h \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/per/sensors.h

HEADERS += \
    $$LIB_EXP_DIR/async/temperature_sensors_simulator/alg/dispatcher.h

