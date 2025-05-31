include (../../../common.pri)

TEMPLATE=subdirs

PRJ_DIR=$$BASE_DIR/tnct/mf4/v411

HEADERS += \
    $$PRJ_DIR/mem/block.h  \
    $$PRJ_DIR/mem/block_id.h  \
    $$PRJ_DIR/mem/block_index.h  \
    $$PRJ_DIR/mem/block_ref.h  \
    $$PRJ_DIR/mem/data_section.h  \
    $$PRJ_DIR/mem/data_section_hd.h  \
    $$PRJ_DIR/mem/data_section_md.h  \
    $$PRJ_DIR/mem/data_section_tx.h  \
    $$PRJ_DIR/mem/data_section_fh.h  \
    $$PRJ_DIR/mem/data_section_ch.h  \
    $$PRJ_DIR/mem/data_section_at.h  \
    $$PRJ_DIR/mem/id_block.h \
    $$PRJ_DIR/mem/file.h
