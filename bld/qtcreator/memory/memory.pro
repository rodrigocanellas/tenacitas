include (../common.pri)

TEMPLATE=aux

PRJ_DIR=$$BASE_DIR/tnct/memory

HEADERS += \
    $$PRJ_DIR/traits/has_new_operator.h \
    $$PRJ_DIR/traits/is_smart_ptr.h
