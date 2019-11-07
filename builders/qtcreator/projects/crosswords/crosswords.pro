SUBDIRS = \
  dependencies \
  entities/crosswords_entities.pro \
  messages/crosswords_messages.pro \
  business/crosswords_business.pro \
  test/crosswords_test.pro



TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered




















