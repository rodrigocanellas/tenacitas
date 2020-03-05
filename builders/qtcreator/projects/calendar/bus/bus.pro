CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

VERSION = 0.0.1

TARGET = tenacitas.calendar.bus




include (../../../common.pri)

HEADERS += \
  ../../../../../code/calendar/bus/create_sequence.h \
  ../../../../../code/calendar/bus/daily_repetition.h \
  ../../../../../code/calendar/bus/monthly_repetition.h \
  ../../../../../code/calendar/bus/weekly_repetition.h \
  ../../../../../code/calendar/bus/yearly_repetition.h \
  ../../../../../code/calendar/bus/ending_after.h \
  ../../../../../code/calendar/bus/ending_never.h \
  ../../../../../code/calendar/bus/ending_on.h \
