QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.crosswords.test.application.app_001

CONFIG+=test


include (../../../../common.pri)

LIBS += -ltenacitas.logger.business -ltenacitas.crosswords.entities -ltenacitas.crosswords.business


SOURCES += \
    ../../../../../../code/crosswords/test/application/app_001/main.cpp

DISTFILES += \
    ../../../../../../code/crosswords/test/application/app_001/crosswords_000.txt \
    ../../../../../../code/crosswords/test/application/app_001/crosswords_001.txt \
    ../../../../../../code/crosswords/test/application/app_001/crosswords_002.txt \
    ../../../../../../code/crosswords/test/application/app_001/crosswords_003.txt \
    ../../../../../../code/crosswords/test/application/app_001/crosswords_004.txt \
    ../../../../../../code/crosswords/test/application/app_001/crosswords_005.txt


