QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = tenacitas.messages.crosswords



include(../../../common.pri)


LIBS += -ltenacitas.entities.crosswords

HEADERS += \
    ../../../../../code/messages/crosswords/not_positioned.h \
    ../../../../../code/messages/crosswords/positioned.h \
    ../../../../../code/messages/crosswords/stop_positioning.h \
    ../../../../../code/messages/crosswords/to_position.h


