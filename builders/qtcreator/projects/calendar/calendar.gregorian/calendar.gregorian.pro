CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.calendar.gregorian


include (../../../common.pri)

HEADERS += \
  $$code_src_dir/calendar/gregorian/second.h  \
  $$code_src_dir/calendar/gregorian/minute.h \
  $$code_src_dir/calendar/gregorian/hour.h \
  $$code_src_dir/calendar/gregorian/day.h \
  $$code_src_dir/calendar/gregorian/weekday.h \
  $$code_src_dir/calendar/gregorian/month.h \
  $$code_src_dir/calendar/gregorian/year.h \
  $$code_src_dir/calendar/gregorian/amount.h \
  $$code_src_dir/calendar/gregorian/seconds.h \
  $$code_src_dir/calendar/gregorian/days.h \
  $$code_src_dir/calendar/gregorian/hours.h \
  $$code_src_dir/calendar/gregorian/minutes.h \
  $$code_src_dir/calendar/gregorian/months.h \
  $$code_src_dir/calendar/gregorian/weekdays.h \
  $$code_src_dir/calendar/gregorian/years.h \
  $$code_src_dir/calendar/gregorian/week.h \
  $$code_src_dir/calendar/gregorian/weeks.h \
  $$code_src_dir/calendar/gregorian/add.h \
  ../../../../../src/calendar/gregorian/convert.h

#\
#  $$code_src_dir/calendar/gregorian/amount.h \
#  $$code_src_dir/calendar/gregorian/seconds.h \
#  $$code_src_dir/calendar/gregorian/days.h \
#  $$code_src_dir/calendar/gregorian/hours.h \
#  $$code_src_dir/calendar/gregorian/minutes.h \
#  $$code_src_dir/calendar/gregorian/months.h \
#  $$code_src_dir/calendar/gregorian/weekdays.h \
#  $$code_src_dir/calendar/gregorian/years.h  \
#  $$code_src_dir/calendar/gregorian/create_sequence.h \
#  $$code_src_dir/calendar/gregorian/daily_repetition.h \
#  $$code_src_dir/calendar/gregorian/ending_after.h \
#  $$code_src_dir/calendar/gregorian/ending_never.h \
#  $$code_src_dir/calendar/gregorian/ending_on.h \
#  $$code_src_dir/calendar/gregorian/monthly_repetition.h \
#  $$code_src_dir/calendar/gregorian/weekly_repetition.h \
#  $$code_src_dir/calendar/gregorian/yearly_repetition.h \
#  $$code_src_dir/calendar/gregorian/timestamp.h \


SOURCES += \
  $$code_src_dir/calendar/gregorian/second.cpp \
  $$code_src_dir/calendar/gregorian/minute.cpp \
  $$code_src_dir/calendar/gregorian/hour.cpp \
  $$code_src_dir/calendar/gregorian/day.cpp \
  $$code_src_dir/calendar/gregorian/weekday.cpp
#  \
#  \
#  $$code_src_dir/calendar/gregorian/month.cpp \
#  $$code_src_dir/calendar/gregorian/timestamp.cpp

