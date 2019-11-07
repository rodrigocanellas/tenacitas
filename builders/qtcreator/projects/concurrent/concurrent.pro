SUBDIRS = \
  business/concurrent_business.pro \
  test/concurrent_test.pro

TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG +=     ordered
















