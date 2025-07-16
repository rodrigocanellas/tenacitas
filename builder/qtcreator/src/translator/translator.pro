include (../../common.pri)


TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
        $$BASE_SRC_DIR/tnct/translator/translator_from_file.h \
        $$BASE_SRC_DIR/tnct/translator/translator_in_memory.h \
        $$BASE_SRC_DIR/tnct/translator/dictionary.h \
        $$BASE_SRC_DIR/tnct/translator/translate.h




