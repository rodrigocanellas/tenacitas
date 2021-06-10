QT -= core
TEMPLATE = app
TARGET = tenacitas.program.exp.application_000
CONFIG+=example
include (../../../tenacitas.lib.pri)



SOURCES += \
  $$exp_code_dir/exp/program/application_000/main.cpp
