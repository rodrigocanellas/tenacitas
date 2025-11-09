include (../common.pri)


TEMPLATE=subdirs

SUBDIRS = tst

HEADERS += \
        $$BASE_DIR/tnct/translator/bus/translator_from_file.h \
        $$BASE_DIR/tnct/translator/bus/translator_in_memory.h \
        $$BASE_DIR/tnct/translator/dat/dictionary.h \
        $$BASE_DIR/tnct/translator/dat/translate.h




