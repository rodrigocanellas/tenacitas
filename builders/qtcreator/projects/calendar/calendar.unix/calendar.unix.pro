CONFIG -= qt


TEMPLATE = lib
CONFIG += static

#VERSION = 0.0.1

TARGET = tenacitas.calendar.unix

include (../../../common.pri)

#LIBS += $$libs_dir/libtenacitas.calendar.ent.a

HEADERS += \
    ../../../../../code/calendar/bus/conversions.h \
    ../../../../../code/calendar/bus/create_sequence.h \
    ../../../../../code/calendar/bus/daily_repetition.h \
    ../../../../../code/calendar/bus/ending_after.h \
    ../../../../../code/calendar/bus/ending_never.h \
    ../../../../../code/calendar/bus/ending_on.h \
    ../../../../../code/calendar/bus/epoch.h \
    ../../../../../code/calendar/bus/monthly_repetition.h \
    ../../../../../code/calendar/bus/weekly_repetition.h \
    ../../../../../code/calendar/bus/yearly_repetition.h \
    ../../../../../code/calendar/ent/amount.h \
    ../../../../../code/calendar/ent/day.h \
    ../../../../../code/calendar/ent/days.h \
    ../../../../../code/calendar/ent/hour.h \
    ../../../../../code/calendar/ent/hours.h \
    ../../../../../code/calendar/ent/minute.h \
    ../../../../../code/calendar/ent/minutes.h \
    ../../../../../code/calendar/ent/month.h \
    ../../../../../code/calendar/ent/months.h \
    ../../../../../code/calendar/ent/second.h \
    ../../../../../code/calendar/ent/seconds.h \
    ../../../../../code/calendar/ent/weekday.h \
    ../../../../../code/calendar/ent/weekdays.h \
    ../../../../../code/calendar/ent/year.h \
    ../../../../../code/calendar/ent/years.h \
    ../../../../../code/calendar/unix/create_sequence.h \
    ../../../../../code/calendar/unix/endings.h \
    ../../../../../code/calendar/unix/repetitions.h \
    ../../../../../code/calendar/unix/timestamp.h

SOURCES += \
    ../../../../../code/calendar/ent/day.cpp \
    ../../../../../code/calendar/ent/hour.cpp \
    ../../../../../code/calendar/ent/minute.cpp \
    ../../../../../code/calendar/ent/month.cpp \
    ../../../../../code/calendar/ent/second.cpp \
    ../../../../../code/calendar/ent/weekday.cpp \
    ../../../../../code/calendar/unix/internal/timestamp.cpp
