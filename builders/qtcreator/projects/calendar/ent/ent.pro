CONFIG -= qt


TEMPLATE = lib
CONFIG += staticlib

#VERSION = 0.0.1

TARGET = tenacitas.calendar.ent


HEADERS += \
  ../../../../../code/calendar/ent/internal/amount.h \
  ../../../../../code/calendar/ent/second.h \
  ../../../../../code/calendar/ent/seconds.h \
  ../../../../../code/calendar/ent/minute.h \
  ../../../../../code/calendar/ent/minutes.h \
  ../../../../../code/calendar/ent/hour.h \
  ../../../../../code/calendar/ent/hours.h \
  ../../../../../code/calendar/ent/day.h \
  ../../../../../code/calendar/ent/days.h \
  ../../../../../code/calendar/ent/weekday.h \
  ../../../../../code/calendar/ent/weekdays.h \
  ../../../../../code/calendar/ent/month.h \
  ../../../../../code/calendar/ent/months.h \
  ../../../../../code/calendar/ent/year.h \
  ../../../../../code/calendar/ent/years.h \


SOURCES += \
  ../../../../../code/calendar/ent/internal/day.cpp \
  ../../../../../code/calendar/ent/internal/hour.cpp \
  ../../../../../code/calendar/ent/internal/minute.cpp \
  ../../../../../code/calendar/ent/internal/month.cpp \
  ../../../../../code/calendar/ent/internal/second.cpp \
  ../../../../../code/calendar/ent/internal/weekday.cpp



include (../../../common.pri)
