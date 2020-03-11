CONFIG -= qt


TEMPLATE = lib
CONFIG += static

#VERSION = 0.0.1

TARGET = tenacitas.calendar.unix

HEADERS += \
    ../../../../../code/calendar/bus/create_sequence.h \
    ../../../../../code/calendar/bus/daily_repetition.h \
    ../../../../../code/calendar/bus/ending_after.h \
    ../../../../../code/calendar/bus/ending_never.h \
    ../../../../../code/calendar/bus/ending_on.h \
    ../../../../../code/calendar/bus/monthly_repetition.h \
    ../../../../../code/calendar/bus/weekly_repetition.h \
    ../../../../../code/calendar/bus/yearly_repetition.h \
    ../../../../../code/calendar/ent/amount.h \
    ../../../../../code/calendar/ent/timestamp_unix.h \
    ../../../../../code/calendar/ent/day.h \
    ../../../../../code/calendar/ent/hour.h \
    ../../../../../code/calendar/ent/minute.h \
    ../../../../../code/calendar/ent/month.h \
    ../../../../../code/calendar/ent/second.h \
    ../../../../../code/calendar/ent/weekday.h \
    ../../../../../code/calendar/ent/year.h \
    ../../../../../code/calendar/unix/amounts.h \
    ../../../../../code/calendar/unix/create_sequence.h \
    ../../../../../code/calendar/unix/endings.h \
    ../../../../../code/calendar/unix/repetitions.h \
    ../../../../../code/calendar/unix/times.h \
    ../../../../../code/calendar/unix/timestamp.h

SOURCES += \
    ../../../../../code/calendar/ent/internal/day.cpp \
    ../../../../../code/calendar/ent/internal/hour.cpp \
    ../../../../../code/calendar/ent/internal/minute.cpp \
    ../../../../../code/calendar/ent/internal/month.cpp \
    ../../../../../code/calendar/ent/internal/second.cpp \
    ../../../../../code/calendar/ent/internal/weekday.cpp \
    ../../../../../code/calendar/ent/internal/timestamp_unix.cpp
# \
#    ../../../../../code/calendar/unix/internal/timestamp.cpp

include (../../../common.pri)

#LIBS += $$libs_dir/libtenacitas.calendar.ent.a
