SUBDIRS = \
    ../concurrent/library/business/concurrent_library_business.pro \
    library/logger_library.pro \
    test/logger_test.pro

TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

