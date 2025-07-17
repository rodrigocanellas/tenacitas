QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.time

include (../../common.pri)


SOURCES = $$BASE_DIR_TST/tnct/time/main.cpp

HEADERS += $$BASE_DIR_TST/tnct/time/chrono_output_test.h
