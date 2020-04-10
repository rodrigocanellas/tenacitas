

CONFIG -= qt
TEMPLATE = app
CONFIG += test
TARGET = tenacitas.options.test.options000


SOURCES += \
  $$code_src_dir/pgm_options/tst/options000/main.cpp

include (../../../../common.pri)

LIBS += -ltenacitas.logger.business
LIBS += $$libs_dir/libtenacitas.options.bus.$$static_lib_ext




