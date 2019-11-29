#-------------------------------------------------
#
# Project created by QtCreator 2019-11-29T16:08:25
#
#-------------------------------------------------

QT       -= core gui

TARGET = capemisa.sql.entities
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#INCLUDEPATH += D:\c++\tenacitas\code

SOURCES += \
    ../../../../code/sql/entities/internal/attribute_column.cpp \
    ../../../../code/sql/entities/internal/column.cpp \
    ../../../../code/sql/entities/internal/database.cpp \
    ../../../../code/sql/entities/internal/foreign_key.cpp \
    ../../../../code/sql/entities/internal/foreign_key_column.cpp \
    ../../../../code/sql/entities/internal/host.cpp \
    ../../../../code/sql/entities/internal/primary_key.cpp \
    ../../../../code/sql/entities/internal/primary_key_column.cpp \
    ../../../../code/sql/entities/internal/server.cpp \
    ../../../../code/sql/entities/internal/table.cpp

HEADERS += \
    ../../../../code/sql/entities/attribute_column.h \
    ../../../../code/sql/entities/category.h \
    ../../../../code/sql/entities/column.h \
    ../../../../code/sql/entities/column_value.h \
    ../../../../code/sql/entities/database.h \
    ../../../../code/sql/entities/foreign_key.h \
    ../../../../code/sql/entities/foreign_key_column.h \
    ../../../../code/sql/entities/host.h \
    ../../../../code/sql/entities/hosts.h \
    ../../../../code/sql/entities/internal/collection.h \
    ../../../../code/sql/entities/internal/types.h \
    ../../../../code/sql/entities/ip.h \
    ../../../../code/sql/entities/name.h \
    ../../../../code/sql/entities/primary_key.h \
    ../../../../code/sql/entities/primary_key_column.h \
    ../../../../code/sql/entities/server.h \
    ../../../../code/sql/entities/size.h \
    ../../../../code/sql/entities/table.h \
    ../../../../code/sql/entities/value_generator.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}


include(../../common.pri)
