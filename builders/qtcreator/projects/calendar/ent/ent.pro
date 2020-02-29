CONFIG -= qt


TEMPLATE = lib
CONFIG += staticlib

VERSION = 0.0.1

TARGET = tenacitas.calendar.ent


HEADERS += \
  ../../../../../code/calendar/ent/amount.h \
  ../../../../../code/calendar/ent/day.h \
  ../../../../../code/calendar/ent/ending_after.h \
  ../../../../../code/calendar/ent/ending_never.h \
  ../../../../../code/calendar/ent/ending_on.h \
  ../../../../../code/calendar/ent/hour.h \
  ../../../../../code/calendar/ent/minute.h \
  ../../../../../code/calendar/ent/month.h \
  ../../../../../code/calendar/ent/number_of_days.h \
  ../../../../../code/calendar/ent/recurrency.h \
  ../../../../../code/calendar/ent/recurrency_day.h \
  ../../../../../code/calendar/ent/second.h \
  ../../../../../code/calendar/ent/timestamp.h \
  ../../../../../code/calendar/ent/timestamp_second.h \
  ../../../../../code/calendar/ent/types_of_repetition.h \
  ../../../../../code/calendar/ent/weekday.h \
  ../../../../../code/calendar/ent/year.h


SOURCES += \
  ../../../../../code/calendar/ent/internal/day.cpp \
  ../../../../../code/calendar/ent/internal/hour.cpp \
  ../../../../../code/calendar/ent/internal/minute.cpp \
  ../../../../../code/calendar/ent/internal/month.cpp \
  ../../../../../code/calendar/ent/internal/second.cpp \
  ../../../../../code/calendar/ent/internal/timestamp_second.cpp \
  ../../../../../code/calendar/ent/internal/weekday.cpp



include (../../../common.pri)
