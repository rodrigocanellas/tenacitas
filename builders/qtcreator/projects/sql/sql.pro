TEMPLATE = subdirs

SUBDIRS += entities/sql_entities.pro \
           business/sql_business.pro \
           test/sql_test.pro

CONFIG += ordered
