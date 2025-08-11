include (../../../../../common.pri)

QT += core gui widgets

CONFIG += c++20 app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0




SOURCES += \
    $$BASE_DIR/tenacitas/src/crosswords/uix/qt/author_desktop/main.cpp \
    $$BASE_DIR/tenacitas/src/crosswords/uix/qt/author_desktop/mainwindow.cpp
HEADERS += \
    $$BASE_DIR/tenacitas/src/crosswords/uix/qt/author_desktop/mainwindow.h

FORMS += \
    $$BASE_DIR/tenacitas/src/crosswords/uix/qt/author_desktop/mainwindow.ui



#TRANSLATIONS += \
#    desktop_pt_BR.ts
#CONFIG += lrelease
#CONFIG += embed_translations


TARGET=tenacitas.crosswords.qt.desktop

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $$BASE_DIR/tenacitas/src/crosswords/rsc/crosswords.qrc
