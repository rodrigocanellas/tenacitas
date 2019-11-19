QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = capemisa.sql.entities

HEADERS += \
    ../../../../../code/sql/entities/column_value_pattern.h \
    ../../../../../code/sql/entities/columns_values.h \
    ../../../../../code/sql/entities/databases.h \
    ../../../../../code/sql/entities/foreigners_keys_values.h \
    ../../../../../code/sql/entities/inserts_definitions.h \
    ../../../../../code/sql/entities/internal/collection.h \
    ../../../../../code/sql/entities/column.h \
    ../../../../../code/sql/entities/columns.h \
    ../../../../../code/sql/entities/database.h \
    ../../../../../code/sql/entities/foreign_key.h \
    ../../../../../code/sql/entities/foreign_key_value.h \
    ../../../../../code/sql/entities/foreign_key_value_pattern.h \
    ../../../../../code/sql/entities/foreigners_keys.h \
    ../../../../../code/sql/entities/host.h \
    ../../../../../code/sql/entities/hosts.h \
    ../../../../../code/sql/entities/ip.h \
    ../../../../../code/sql/entities/name.h \
    ../../../../../code/sql/entities/names.h \
    ../../../../../code/sql/entities/primary_key.h \
    ../../../../../code/sql/entities/server.h \
    ../../../../../code/sql/entities/servers.h \
    ../../../../../code/sql/entities/size.h \
    ../../../../../code/sql/entities/table.h \
    ../../../../../code/sql/entities/tables.h \
    ../../../../../code/sql/entities/types.h \
    ../../../../../code/sql/entities/insert_definition.h \
    ../../../../../code/sql/entities/column_value.h

include (../../../common.pri)

SOURCES += \
  ../../../../../code/sql/entities/internal/column.cpp \
  ../../../../../code/sql/entities/internal/database.cpp \
  ../../../../../code/sql/entities/internal/foreign_key.cpp \
  ../../../../../code/sql/entities/internal/host.cpp \
  ../../../../../code/sql/entities/internal/primary_key.cpp \
  ../../../../../code/sql/entities/internal/server.cpp \
  ../../../../../code/sql/entities/internal/table.cpp

