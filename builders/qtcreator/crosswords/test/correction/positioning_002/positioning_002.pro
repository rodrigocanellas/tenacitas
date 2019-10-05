QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.crosswords.test.correction.positioning_002

CONFIG+=test


include (../../../../common.pri)

LIBS += -ltenacitas.logger.business -ltenacitas.crosswords.entities -ltenacitas.crosswords.business


SOURCES += \
    ../../../../../../code/crosswords/test/correction/positioning_002/main.cpp


