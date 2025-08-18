TEMPLATE=subdirs

QMAKE_CXXFLAGS += -DTENACITAS_LOG

SUBDIRS = \
    bus \
    evt \
    dat \
    tst \
    app
