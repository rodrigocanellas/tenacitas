CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.calendar.gregorian


include (../../../common.pri)

HEADERS += \
  $$code_src_dir/calendar/_ent/amount.h \
  $$code_src_dir/calendar/_ent/day.h \
  $$code_src_dir/calendar/_ent/days.h \
  $$code_src_dir/calendar/_ent/hour.h \
  $$code_src_dir/calendar/_ent/hours.h \
  $$code_src_dir/calendar/_ent/minute.h \
  $$code_src_dir/calendar/_ent/minutes.h \
  $$code_src_dir/calendar/_ent/month.h \
  $$code_src_dir/calendar/_ent/months.h \
  $$code_src_dir/calendar/_ent/second.h \
  $$code_src_dir/calendar/_ent/seconds.h \
  $$code_src_dir/calendar/_ent/weekday.h \
  $$code_src_dir/calendar/_ent/weekdays.h \
  $$code_src_dir/calendar/_ent/year.h \
  $$code_src_dir/calendar/_ent/years.h \
  $$code_src_dir/calendar/gregorian/timestamp.h

SOURCES += \
  $$code_src_dir/calendar/_ent/day.cpp \
  $$code_src_dir/calendar/_ent/hour.cpp \
  $$code_src_dir/calendar/_ent/minute.cpp \
  $$code_src_dir/calendar/_ent/month.cpp \
  $$code_src_dir/calendar/_ent/second.cpp \
  $$code_src_dir/calendar/_ent/weekday.cpp
