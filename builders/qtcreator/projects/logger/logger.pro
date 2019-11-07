SUBDIRS = \
    dependencies/dependencies.pro \
    business/logger_business.pro \
    test/logger_test.pro

TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered


