CONFIG -= qt


TEMPLATE = lib
CONFIG += static

#VERSION = 0.0.1

TARGET = tenacitas.calendar.unix

include (../../../common.pri)

#LIBS += $$libs_dir/libtenacitas.calendar._ent.a

HEADERS += \
    $$code_src_dir/calendar/_bus/add.h \
    $$code_src_dir/calendar/_bus/conversions.h \
    $$code_src_dir/calendar/_bus/create_sequence.h \
    $$code_src_dir/calendar/_bus/daily_repetition.h \
    $$code_src_dir/calendar/_bus/ending_after.h \
    $$code_src_dir/calendar/_bus/ending_never.h \
    $$code_src_dir/calendar/_bus/ending_on.h \
    $$code_src_dir/calendar/_bus/epoch.h \
    $$code_src_dir/calendar/_bus/monthly_repetition.h \
    $$code_src_dir/calendar/_bus/weekly_repetition.h \
    $$code_src_dir/calendar/_bus/yearly_repetition.h \
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
    $$code_src_dir/calendar/unix/create_sequence.h \
    $$code_src_dir/calendar/unix/endings.h \
    $$code_src_dir/calendar/unix/repetitions.h \
    $$code_src_dir/calendar/unix/timestamp.h

SOURCES += \
    $$code_src_dir/calendar/_ent/day.cpp \
    $$code_src_dir/calendar/_ent/hour.cpp \
    $$code_src_dir/calendar/_ent/minute.cpp \
    $$code_src_dir/calendar/_ent/month.cpp \
    $$code_src_dir/calendar/_ent/second.cpp \
    $$code_src_dir/calendar/_ent/weekday.cpp \
    $$code_src_dir/calendar/unix/internal/timestamp.cpp
