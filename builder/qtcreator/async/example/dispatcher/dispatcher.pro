include (../../../common.pri)
TEMPLATE=subdirs
QMAKE_CXXFLAGS += -DTENACITAS_LOG
SUBDIRS = \
         dispatcher_000 \
         dispatcher_009



HEADERS+=$$BASE_DIR/tenacitas.lib/async/exp/dispatcher/publisher.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/logger.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/event.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/handler.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/event_handled.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/event_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/handler_type_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/handler_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/handling_id.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/results.h \
         $$BASE_DIR/tenacitas.lib/async/exp/dispatcher/configuration.h

# \
        # temperature_sensors_simulator
