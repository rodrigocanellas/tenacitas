QT += core widgets
TEMPLATE = app
TARGET = tenacitas.lib.exp.temperature_sensors_simulator
CONFIG += example
include (../../common.pri)

SOURCES += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/uix/qt/main.cpp \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/uix/qt/mainwindow.cpp

FORMS += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/uix/qt/mainwindow.ui

HEADERS += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/uix/qt/mainwindow.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/sensor_id.h \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/temperature.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/asy/add_sensor.h \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/asy/new_temperature.h \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/asy/remove_sensor.h \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/asy/set_temperature.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/per/sensor.h \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/per/sensors.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/exp/temperature_sensors_simulator/asy/dispatcher.h

