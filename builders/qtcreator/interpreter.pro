SUBDIRS = \
    string/library/string_library.pro \
    logger/library/logger_library.pro \
    interpreter/library/interpreter_library.pro \
    interpreter/test/interpreter_test.pro


TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
















