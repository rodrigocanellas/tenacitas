CONFIG -= qt


TEMPLATE = lib
CONFIG += static

#VERSION = 0.0.1

TARGET = tenacitas.calendar.unix

include (../../../common.pri)

#LIBS += $$libs_dir/libtenacitas.calendar._ent.a

HEADERS += \
    ../../../../../code/calendar/_bus/conversions.h \
    ../../../../../code/calendar/_bus/create_sequence.h \
    ../../../../../code/calendar/_bus/daily_repetition.h \
    ../../../../../code/calendar/_bus/ending_after.h \
    ../../../../../code/calendar/_bus/ending_never.h \
    ../../../../../code/calendar/_bus/ending_on.h \
    ../../../../../code/calendar/_bus/epoch.h \
    ../../../../../code/calendar/_bus/monthly_repetition.h \
    ../../../../../code/calendar/_bus/weekly_repetition.h \
    ../../../../../code/calendar/_bus/yearly_repetition.h \
    ../../../../../code/calendar/_ent/amount.h \
    ../../../../../code/calendar/_ent/day.h \
    ../../../../../code/calendar/_ent/days.h \
    ../../../../../code/calendar/_ent/hour.h \
    ../../../../../code/calendar/_ent/hours.h \
    ../../../../../code/calendar/_ent/minute.h \
    ../../../../../code/calendar/_ent/minutes.h \
    ../../../../../code/calendar/_ent/month.h \
    ../../../../../code/calendar/_ent/months.h \
    ../../../../../code/calendar/_ent/second.h \
    ../../../../../code/calendar/_ent/seconds.h \
    ../../../../../code/calendar/_ent/weekday.h \
    ../../../../../code/calendar/_ent/weekdays.h \
    ../../../../../code/calendar/_ent/year.h \
    ../../../../../code/calendar/_ent/years.h \
    ../../../../../code/calendar/unix/create_sequence.h \
    ../../../../../code/calendar/unix/endings.h \
    ../../../../../code/calendar/unix/repetitions.h \
    ../../../../../code/calendar/unix/timestamp.h

SOURCES += \
    ../../../../../code/calendar/_ent/day.cpp \
    ../../../../../code/calendar/_ent/hour.cpp \
    ../../../../../code/calendar/_ent/minute.cpp \
    ../../../../../code/calendar/_ent/month.cpp \
    ../../../../../code/calendar/_ent/second.cpp \
    ../../../../../code/calendar/_ent/weekday.cpp \
    ../../../../../code/calendar/unix/internal/timestamp.cpp
