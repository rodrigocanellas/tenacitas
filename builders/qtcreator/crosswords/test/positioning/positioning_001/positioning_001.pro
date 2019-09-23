QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.crosswords.test.positioning.positioning_001

CONFIG+=test


include (../../../../common.pri)

LIBS += -ltenacitas.logger -ltenacitas.crosswords.entities -ltenacitas.crosswords.business

SOURCES += \
    ../../../../../../code/crosswords/test/positioning/positioning_001/main.cpp


