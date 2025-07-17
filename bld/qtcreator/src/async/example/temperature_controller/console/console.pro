QT -= core
TEMPLATE = app
TARGET = tenacitas.lib.async.example.temperature_controller.console
CONFIG += example
include (../../../../common.pri)
SOURCES = $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/console/main.cpp


HEADERS += \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/typ/temperature.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/typ/sensor_id.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/evt/set_temperature.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/evt/temperature_measured.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/evt/temperature_target.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/evt/sensors_installed.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/dev/temperature_sensor.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/dev/temperature_sensors.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/bus/temperature_controller.h \
    $$BASE_DIR/tenacitas.lib/async/example/temperature_controller/uix/text_ui.h


