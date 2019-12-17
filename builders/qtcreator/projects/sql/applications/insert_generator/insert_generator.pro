QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

include (../../../../common.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
  ../../../../../../code/sql/applications/insert_generator/fixed_value_generator.ui \
  ../../../../../../code/sql/applications/insert_generator/mainwindow.ui \
  ../../../../../../code/sql/applications/insert_generator/number_value_generator_definition.ui \
  ../../../../../../code/sql/applications/insert_generator/show_sql.ui \
  ../../../../../../code/sql/applications/insert_generator/table_insert_generator.ui \
  ../../../../../../code/sql/applications/insert_generator/text_value_generator_definition.ui

HEADERS += \
  ../../../../../../code/sql/applications/insert_generator/fixed_value_generator.h \
  ../../../../../../code/sql/applications/insert_generator/generator_definition_window_factory.h \
  ../../../../../../code/sql/applications/insert_generator/mainwindow.h \
  ../../../../../../code/sql/applications/insert_generator/number_value_generator_definition.h \
  ../../../../../../code/sql/applications/insert_generator/show_sql.h \
  ../../../../../../code/sql/applications/insert_generator/table_insert_generator.h \
  ../../../../../../code/sql/applications/insert_generator/text_value_generator_definition.h

SOURCES += \
  ../../../../../../code/sql/applications/insert_generator/fixed_value_generator.cpp \
  ../../../../../../code/sql/applications/insert_generator/main.cpp \
  ../../../../../../code/sql/applications/insert_generator/mainwindow.cpp \
  ../../../../../../code/sql/applications/insert_generator/number_value_generator_definition.cpp \
  ../../../../../../code/sql/applications/insert_generator/show_sql.cpp \
  ../../../../../../code/sql/applications/insert_generator/table_insert_generator.cpp \
  ../../../../../../code/sql/applications/insert_generator/text_value_generator_definition.cpp

TRANSLATIONS += \
    insert_generator_pt_BR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += -lcapemisa.sql.entities
LIBS += -lcapemisa.sql.business

RESOURCES += \
  ../../../../../../code/sql/resources/capemisa.qrc

DISTFILES += \
  ../../../../../../code/sql/resources/capemisa_pequeno.png
