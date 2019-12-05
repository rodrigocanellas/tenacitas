QT -= core
QT -= widgets
QT -= network
QT -= gui


TEMPLATE = lib

CONFIG += lib

TARGET = capemisa.sql.business


include (../../../common.pri)

HEADERS += \
  ../../../../../code/sql/business/attribute_column_generator.h \
  ../../../../../code/sql/business/foreign_key_generator.h \
  ../../../../../code/sql/business/one_pk_all_fks.h \
  ../../../../../code/sql/business/primary_key_column_generator.h \
  ../../../../../code/sql/business/sequential_value_generator.h \
  ../../../../../code/sql/business/table_generator.h \
  ../../../../../code/sql/generic/ptr.h \
  ../../../../../code/sql/generic/collection.h \
    ../../../../../code/sql/generic/name.h \






