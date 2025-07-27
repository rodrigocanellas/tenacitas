CONFIG += qt core
QT+=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.crosswords

include (../../common.pri)

SOURCES = $$BASE_DIR/tenacitas/tst/crosswords/main.cpp


HEADERS = $$BASE_DIR/tenacitas/tst/crosswords/organizer_test.h
# \
#           $$BASE_DIR/tenacitas/tst/tnct/crosswords/dat_test.h
