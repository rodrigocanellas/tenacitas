include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst

PRJ_DIR=$$BASE_DIR/tnct/interpreter

HEADERS += \
        $$PRJ_DIR/bus/*.h \
        $$PRJ_DIR/dat/*.h \
        $$PRJ_DIR/cpt/*.h

SOURCES += \
        $$PRJ_DIR/bus/*.cpp \
        $$PRJ_DIR/dat/*.cpp \
