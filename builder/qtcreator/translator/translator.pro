include (../common.pri)


TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
        $$BASE_DIR/tenacitas.lib/translator/translator_from_file.h \
        $$BASE_DIR/tenacitas.lib/translator/translator_in_memory.h \
        $$BASE_DIR/tenacitas.lib/translator/dictionary.h \
        $$BASE_DIR/tenacitas.lib/translator/translate.h




