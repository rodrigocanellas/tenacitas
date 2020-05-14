CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.calendar.gregorian


include (../../../common.pri)

HEADERS += \
  $$code_src_dir/calendar/gregorian/second.h \
  $$code_src_dir/calendar/gregorian/minute.h \
  $$code_src_dir/calendar/gregorian/hour.h \
  $$code_src_dir/calendar/gregorian/day.h \
  $$code_src_dir/calendar/gregorian/month.h \
  $$code_src_dir/calendar/gregorian/year.h \
  $$code_src_dir/calendar/gregorian/weekday.h \
  $$code_src_dir/calendar/gregorian/week.h \
  $$code_src_dir/calendar/gregorian/internal/amount.h \
  $$code_src_dir/calendar/gregorian/internal/convert.h \
  $$code_src_dir/calendar/gregorian/amounts.h  \
  $$code_src_dir/calendar/gregorian/timestamp.h


#  $$code_src_dir/calendar/gregorian/add.h \
#  $$code_src_dir/calendar/gregorian/create_sequence.h \
#  $$code_src_dir/calendar/gregorian/daily_repetition.h \
#  $$code_src_dir/calendar/gregorian/ending_after.h \
#  $$code_src_dir/calendar/gregorian/ending_never.h \
#  $$code_src_dir/calendar/gregorian/ending_on.h \
#  $$code_src_dir/calendar/gregorian/monthly_repetition.h \
#  $$code_src_dir/calendar/gregorian/weekly_repetition.h \
#  $$code_src_dir/calendar/gregorian/yearly_repetition.h \



SOURCES += \
  $$code_src_dir/calendar/gregorian/internal/second.cpp \
  $$code_src_dir/calendar/gregorian/internal/minute.cpp \
  $$code_src_dir/calendar/gregorian/internal/hour.cpp \
  $$code_src_dir/calendar/gregorian/internal/day.cpp \
  $$code_src_dir/calendar/gregorian/internal/month.cpp \
  $$code_src_dir/calendar/gregorian/internal/weekday.cpp \
  $$code_src_dir/calendar/gregorian/internal/timestamp.cpp

