include (../../../../common.pri)

TEMPLATE = subdirs

SUBDIRS = \
    $$builder_dir/concurrent/concurrent.headers \
    $$builder_dir/logger/logger.cerr \
    $$builder_dir/tester/tester.headers \



CONFIG += ordered
