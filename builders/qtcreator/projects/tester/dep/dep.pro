include (../../../common.pri)

SUBDIRS = \
  $$builder_dir/program/program.lib


TEMPLATE = subdirs

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
