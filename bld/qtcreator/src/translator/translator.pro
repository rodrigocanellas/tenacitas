include (../../common.pri)


TEMPLATE=subdirs

SUBDIRS = test

HEADERS += \
        $$BASE_DIR/tenacitas/src/translator/translator_from_file.h \
        $$BASE_DIR/tenacitas/src/translator/translator_in_memory.h \
        $$BASE_DIR/tenacitas/src/translator/dictionary.h \
        $$BASE_DIR/tenacitas/src/translator/translate.h




