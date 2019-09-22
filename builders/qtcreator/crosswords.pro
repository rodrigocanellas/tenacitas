SUBDIRS = \
  logger/library/logger_library.pro \
  crosswords/library/crosswords_library.pro \
  crosswords/test/crosswords_test.pro



TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

















