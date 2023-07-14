TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/typ/channel_id.h \
        $$BASE_DIR/tenacitas.lib/src/communication/typ/connection_id.h \

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/src/communication/alg/message.h

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/concepts.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/message.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/serializer.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/deserializer.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/serializers.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/deserializers.h \
        $$BASE_DIR/tenacitas.lib/src/communication/cpt/io.h

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/connection_closed.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/new_connection.h \
        $$BASE_DIR/tenacitas.lib/src/communication/evt/send_timeout.h

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/alg/in_channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/alg/out_channel.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/deserialize.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/new_message.h \
        $$BASE_DIR/tenacitas.lib/src/communication/internal/evt/send_msg.h


