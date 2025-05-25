include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS=\
     app \
     mem \
     per

PRJ_DIR=$$BASE_DIR/tnct/mf4/v411

HEADERS += \
    $$PRJ_DIR/log_and_throw.h



