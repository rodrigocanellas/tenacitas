QT += core widgets
TEMPLATE = app
TARGET = tenacitas.lib.async.example.temperature_sensors_simulator
CONFIG += example
include (../../../common.pri)

SOURCES += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/uix/qt/main.cpp \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/uix/qt/mainwindow.cpp

FORMS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/uix/qt/mainwindow.ui

HEADERS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/uix/qt/mainwindow.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/typ/sensor_id.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/typ/temperature.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/evt/add_sensor.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/evt/new_temperature.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/evt/remove_sensor.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/evt/set_temperature.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/per/sensor.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/per/sensors.h

HEADERS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_sensors_simulator/supplier.h

