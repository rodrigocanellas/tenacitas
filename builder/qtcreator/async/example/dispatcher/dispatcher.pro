include (../../../common.pri)
TEMPLATE=subdirs
QMAKE_CXXFLAGS += -DTENACITAS_LOG
SUBDIRS = \
         dispatcher_000 \
         dispatcher_009



HEADERS+=$$BASE_DIR/tenacitas.lib/async/exp/dispatcher/publisher.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/logger.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/event.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/handler.h

# \
        # temperature_sensors_simulator
