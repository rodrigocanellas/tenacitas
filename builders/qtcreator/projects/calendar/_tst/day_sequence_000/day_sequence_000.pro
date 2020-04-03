TEMPLATE = app

CONFIG -= qt

CONFIG += test

TARGET = tenacitas.calendar._tst.day_sequence_000

SOURCES += \
  ../../../../../../code/calendar/_ent/day.cpp \
  ../../../../../../code/calendar/_ent/hour.cpp \
  ../../../../../../code/calendar/_ent/minute.cpp \
  ../../../../../../code/calendar/_ent/month.cpp \
  ../../../../../../code/calendar/_ent/second.cpp \
  ../../../../../../code/calendar/_ent/weekday.cpp \
  ../../../../../../code/calendar/_tst/day_sequence_000/main.cpp

include (../../../../common.pri)

LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext
LIBS += $$libs_dir/libtenacitas.calendar.unix.$$static_lib_ext

HEADERS += \
  ../../../../../../code/calendar/_bus/daily_repetition.h \
  ../../../../../../code/calendar/_ent/day.h \
  ../../../../../../code/calendar/_ent/hour.h \
  ../../../../../../code/calendar/_ent/minute.h \
  ../../../../../../code/calendar/_ent/month.h \
  ../../../../../../code/calendar/_ent/second.h \
  ../../../../../../code/calendar/_ent/weekday.h \
  ../../../../../../code/calendar/_ent/year.h


