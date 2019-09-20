SUBDIRS = \
  logger/library/logger_library.pro \
  concurrent/library/concurrent_library.pro \
  concurrent/test/concurrent_test.pro



TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
















