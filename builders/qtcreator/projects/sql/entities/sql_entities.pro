QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = capemisa.sql.entities


include (../../../common.pri)

HEADERS += \
    ../../../../../code/sql/entities/attribute_column.h \
    ../../../../../code/sql/entities/column.h \
    ../../../../../code/sql/entities/column_values.h \
    ../../../../../code/sql/entities/database.h \
    ../../../../../code/sql/entities/foreign_key.h \
    ../../../../../code/sql/entities/foreign_key_column.h \
    ../../../../../code/sql/entities/host.h \
    ../../../../../code/sql/entities/hosts.h \
    ../../../../../code/sql/entities/internal/collection.h \
    ../../../../../code/sql/entities/ip.h \
  ../../../../../code/sql/entities/tables_values.h \
    ../../../../../code/sql/generic/name.h \
    ../../../../../code/sql/entities/primary_key.h \
    ../../../../../code/sql/entities/primary_key_column.h \
    ../../../../../code/sql/entities/server.h \
    ../../../../../code/sql/entities/size.h \
    ../../../../../code/sql/entities/table.h \
  ../../../../../code/sql/entities/table_values.h \
    ../../../../../code/sql/entities/value.h \ \
  ../../../../../code/sql/generic/ptr.h \
  ../../../../../code/sql/generic/collection.h

SOURCES += \
    ../../../../../code/sql/entities/internal/attribute_column.cpp \
    ../../../../../code/sql/entities/internal/column.cpp \
    ../../../../../code/sql/entities/internal/database.cpp \
    ../../../../../code/sql/entities/internal/foreign_key.cpp \
    ../../../../../code/sql/entities/internal/foreign_key_column.cpp \
    ../../../../../code/sql/entities/internal/host.cpp \
    ../../../../../code/sql/entities/internal/primary_key.cpp \
    ../../../../../code/sql/entities/internal/primary_key_column.cpp \
    ../../../../../code/sql/entities/internal/server.cpp \
    ../../../../../code/sql/entities/internal/table.cpp





