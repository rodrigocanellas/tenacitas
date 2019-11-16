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
    ../../../../../code/sql/entities/database.h \
    ../../../../../code/sql/entities/foreign_key.h \
    ../../../../../code/sql/entities/foreigners_keys.h \
    ../../../../../code/sql/entities/name.h \
    ../../../../../code/sql/entities/primary_key.h \
    ../../../../../code/sql/entities/size.h \
    ../../../../../code/sql/entities/table.h \
    ../../../../../code/sql/entities/tables.h

include (../../../common.pri)

SOURCES += \
  ../../../../../code/sql/entities/internal/column.cpp \
  ../../../../../code/sql/entities/internal/database.cpp \
  ../../../../../code/sql/entities/internal/foreign_key.cpp \
  ../../../../../code/sql/entities/internal/foreigners_keys.cpp \
  ../../../../../code/sql/entities/internal/primary_key.cpp \
  ../../../../../code/sql/entities/internal/table.cpp

