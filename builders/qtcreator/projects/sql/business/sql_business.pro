QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = capemisa.sql.business


include (../../../common.pri)

HEADERS += \
  ../../../../../code/sql/business/generate_insert.h \
  ../../../../../code/sql/business/insertion_configuration.h


