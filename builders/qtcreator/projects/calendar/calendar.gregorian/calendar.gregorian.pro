CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.calendar.gregorian


include (../../../common.pri)

HEADERS += \
  $$code_src_dir/measures/time/second.h \
  $$code_src_dir/measures/time/minute.h \
  $$code_src_dir/measures/time/hour.h \
  $$code_src_dir/measures/time/day.h \
  $$code_src_dir/measures/time/month.h \
  $$code_src_dir/measures/time/year.h \
  $$code_src_dir/measures/time/weekday.h \
  $$code_src_dir/measures/time/week.h \
  $$code_src_dir/measures/time/internal/amount.h \
  $$code_src_dir/measures/time/internal/convert.h \
  $$code_src_dir/measures/time/time_amounts.h  \
  $$code_src_dir/calendar/timestamp.h \
  $$code_src_dir/measures/time/millisecond.h


#  $$code_src_dir/measures/time/add.h \
#  $$code_src_dir/measures/time/create_sequence.h \
#  $$code_src_dir/measures/time/daily_repetition.h \
#  $$code_src_dir/measures/time/ending_after.h \
#  $$code_src_dir/measures/time/ending_never.h \
#  $$code_src_dir/measures/time/ending_on.h \
#  $$code_src_dir/measures/time/monthly_repetition.h \
#  $$code_src_dir/measures/time/weekly_repetition.h \
#  $$code_src_dir/measures/time/yearly_repetition.h \



SOURCES += \
  $$code_src_dir/measures/time/internal/second.cpp \
  $$code_src_dir/measures/time/internal/minute.cpp \
  $$code_src_dir/measures/time/internal/hour.cpp \
  $$code_src_dir/measures/time/internal/day.cpp \
  $$code_src_dir/measures/time/internal/month.cpp \
  $$code_src_dir/measures/time/internal/weekday.cpp \
  $$code_src_dir/measures/time/internal/timestamp.cpp

LIBS += $$libs_dir/libtenacitas.measures.time.$$static_lib_ext
