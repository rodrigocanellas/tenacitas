SUBDIRS = \
  string/library/string_library.pro \
  string/test/string_test.pro



TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

