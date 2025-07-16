include (../common.pri)


TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
        $$BASE_DIR/tnct/translator/translator_from_file.h \
        $$BASE_DIR/tnct/translator/translator_in_memory.h \
        $$BASE_DIR/tnct/translator/dictionary.h \
        $$BASE_DIR/tnct/translator/translate.h




