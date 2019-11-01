QT -= core
QT -= widgets
QT -= network
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

TARGET = tenacitas.crosswords.test.correction.positioning_000

CONFIG+=test




LIBS += -ltenacitas.logger.business -ltenacitas.crosswords.entities -ltenacitas.crosswords.business

#SOURCES += \
#    ../../../../../../code/test/crosswords/correction/positioning_000/main.cpp

SOURCES += \
    ../../../../../../../code/test/crosswords/correction/positioning_000/main.cpp

DISTFILES += \
    ../../../../../../../code/test/crosswords/correction/positioning_000/crosswords_000.txt \
    ../../../../../../../code/test/crosswords/correction/positioning_000/crosswords_001.txt \
    ../../../../../../../code/test/crosswords/correction/positioning_000/crosswords_002.txt \
    ../../../../../../../code/test/crosswords/correction/positioning_000/crosswords_003.txt \
    ../../../../../../../code/test/crosswords/correction/positioning_000/crosswords_004.txt \
    ../../../../../../../code/test/crosswords/correction/positioning_000/crosswords_005.txt


include (../../../../../common.pri)
