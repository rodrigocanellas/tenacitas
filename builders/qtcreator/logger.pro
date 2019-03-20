SUBDIRS = \
    concurrent/library/concurrent_library.pro \
    logger/library/logger_library.pro \
    logger/test/logger_test.pro


QT -= core
QT -= widgets
QT -= network
QT -= gui

TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

