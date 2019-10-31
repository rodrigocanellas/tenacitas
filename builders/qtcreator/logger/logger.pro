SUBDIRS = \
    dependencies/dependencies.pro \
    business/business_logger.pro \
    test/test.pro

TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered


