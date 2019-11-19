QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = capemisa.sql.business


include (../../../common.pri)

HEADERS += \
    ../../../../../code/sql/business/insert_generator.h


