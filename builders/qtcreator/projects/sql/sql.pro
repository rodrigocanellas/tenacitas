TEMPLATE = subdirs

SUBDIRS += entities/sql_entities.pro \
           business/sql_business.pro \
#           applications/sql_applications.pro \
           test/sql_test.pro

CONFIG += ordered
