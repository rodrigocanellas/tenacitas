CONFIG += qt core
QT+=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.crosswords

include (../../common.pri)

SOURCES = $$BASE_DIR_TST/tnct/crosswords/main.cpp


HEADERS = $$BASE_DIR_TST/tnct/crosswords/organizer_test.h \
          $$BASE_DIR_TST/tnct/crosswords/dat_test.h
