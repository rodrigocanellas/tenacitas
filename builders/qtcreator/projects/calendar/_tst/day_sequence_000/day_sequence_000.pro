TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar._tst.day_sequence_000

SOURCES += \
  $$code_src_dir/calendar/_ent/day.cpp \
  $$code_src_dir/calendar/_ent/hour.cpp \
  $$code_src_dir/calendar/_ent/minute.cpp \
  $$code_src_dir/calendar/_ent/month.cpp \
  $$code_src_dir/calendar/_ent/second.cpp \
  $$code_src_dir/calendar/_ent/weekday.cpp \
  $$code_src_dir/calendar/_tst/day_sequence_000/main.cpp

include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.calendar.unix.$$static_lib_ext

HEADERS += \
  $$code_src_dir/calendar/_bus/daily_repetition.h \
  $$code_src_dir/calendar/_ent/day.h \
  $$code_src_dir/calendar/_ent/hour.h \
  $$code_src_dir/calendar/_ent/minute.h \
  $$code_src_dir/calendar/_ent/month.h \
  $$code_src_dir/calendar/_ent/second.h \
  $$code_src_dir/calendar/_ent/weekday.h \
  $$code_src_dir/calendar/_ent/year.h


