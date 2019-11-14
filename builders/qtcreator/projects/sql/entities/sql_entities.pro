QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = capemisa.sql.entities

HEADERS += \
    ../../../../../code/sql/entities/column.h \
    ../../../../../code/sql/entities/columns.h \
    ../../../../../code/sql/entities/foreign_key.h \
    ../../../../../code/sql/entities/foreign_keys.h \
    ../../../../../code/sql/entities/name.h \
    ../../../../../code/sql/entities/primary_key.h \
    ../../../../../code/sql/entities/size.h \
    ../../../../../code/sql/entities/table.h \
    ../../../../../code/sql/entities/tables.h \
    ../../../../../code/sql/entities/traits.h

include (../../../common.pri)

SOURCES +=

