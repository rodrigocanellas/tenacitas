SUBDIRS = \
    dependencies/dependencies.pro \
    library/library.pro \
    test/test.pro

TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

