QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.crosswords.test.positioning.positioning_000

CONFIG+=test


include (../../../../common.pri)

LIBS += -ltenacitas.logger -ltenacitas.crosswords

SOURCES += \
    ../../../../../../code/crosswords/test/positioning/positioning_000/main.cpp


