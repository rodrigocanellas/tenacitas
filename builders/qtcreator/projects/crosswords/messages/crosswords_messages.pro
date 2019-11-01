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
    ../../../../../code/crosswords/messages/not_positioned.h \
    ../../../../../code/crosswords/messages/positioned.h \
    ../../../../../code/crosswords/messages/stop_positioning.h \
    ../../../../../code/crosswords/messages/to_position.h


