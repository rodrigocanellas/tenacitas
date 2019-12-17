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
  ../../../../../code/sql/business/attribute_columns_generators_factory.h \
  ../../../../../code/sql/business/column_generator.h \
  ../../../../../code/sql/business/columns_generators_factory.h \
  ../../../../../code/sql/business/fixed_value_generator.h \
  ../../../../../code/sql/business/foreign_key_generator.h \
  ../../../../../code/sql/business/foreign_keys_columns_generator_factory.h \
  ../../../../../code/sql/business/internal/tables_not_populated.h \
  ../../../../../code/sql/business/number_value_generator.h \
  ../../../../../code/sql/business/one_pk_all_fks.h \
  ../../../../../code/sql/business/primary_key_column_generator.h \
  ../../../../../code/sql/business/primary_keys_columns_generator_factory.h \
  ../../../../../code/sql/business/rotate_pks_in_fks.h \
  ../../../../../code/sql/business/sql_generator.h \
  ../../../../../code/sql/business/table_generator.h \
  ../../../../../code/sql/business/text_value_generator.h \
  ../../../../../code/sql/generic/ptr.h \
  ../../../../../code/sql/generic/collection.h \
    ../../../../../code/sql/generic/name.h \ \
  ../../../../../code/sql/generic/string_split.h

SOURCES += \
  ../../../../../code/sql/business/internal/fixed_value_generator.cpp \
  ../../../../../code/sql/business/internal/one_pk_all_fks.cpp \
  ../../../../../code/sql/business/internal/rotate_pks_in_fks.cpp \
  ../../../../../code/sql/business/internal/sql_generator.cpp \
  ../../../../../code/sql/business/internal/tables_not_populated.cpp \
  ../../../../../code/sql/business/internal/text_value_generator.cpp

LIBS += $$libs_dir/libcapemisa.sql.entities.a






