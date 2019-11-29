TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += test

#INCLUDEPATH += D:\c++\tenacitas\code

SOURCES += \
    ../../../../../code/sql/test/print/main.cpp

include(../../../../common.pri)

#LIBS+=-LD:\c++\tenacitas\builders\qt\sql\entities\debug -lcapemisa.sql.entities

LIBS+=-lcapemisa.sql.entities
