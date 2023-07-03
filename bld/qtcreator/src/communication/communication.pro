TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/message.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/concepts.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/connection_closed.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/new_connection.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/send_timeout.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/alg/in_channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/alg/out_channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/deserialize.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/new_message.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/send_msg.h \
        $$BASE_DIR/tenacitas.lib/src/communication/typ/channel_id.h \
        $$BASE_DIR/tenacitas.lib/src/communication/typ/connection_id.h \

