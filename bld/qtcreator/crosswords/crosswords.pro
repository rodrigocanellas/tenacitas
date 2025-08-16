TEMPLATE=subdirs

QMAKE_CXXFLAGS += -DTENACITAS_LOG

SUBDIRS = \
    bus \
    evt \
    dat \
    uix/qt/author_desktop \
    tst
