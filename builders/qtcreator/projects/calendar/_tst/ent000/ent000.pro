TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=test


TARGET = tenacitas.calendar._tst.ent000


include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

HEADERS += \
    ../../../../../../code/calendar/_bus/conversions.h \
    ../../../../../../code/calendar/_bus/create_sequence.h \
    ../../../../../../code/calendar/_bus/daily_repetition.h \
    ../../../../../../code/calendar/_bus/ending_after.h \
    ../../../../../../code/calendar/_bus/ending_never.h \
    ../../../../../../code/calendar/_bus/ending_on.h \
    ../../../../../../code/calendar/_bus/epoch.h \
    ../../../../../../code/calendar/_bus/monthly_repetition.h \
    ../../../../../../code/calendar/_bus/weekly_repetition.h \
    ../../../../../../code/calendar/_bus/yearly_repetition.h \
    ../../../../../../code/calendar/_ent/amount.h \
    ../../../../../../code/calendar/_ent/day.h \
    ../../../../../../code/calendar/_ent/days.h \
    ../../../../../../code/calendar/_ent/hour.h \
    ../../../../../../code/calendar/_ent/hours.h \
    ../../../../../../code/calendar/_ent/minute.h \
    ../../../../../../code/calendar/_ent/minutes.h \
    ../../../../../../code/calendar/_ent/month.h \
    ../../../../../../code/calendar/_ent/months.h \
    ../../../../../../code/calendar/_ent/second.h \
    ../../../../../../code/calendar/_ent/seconds.h \
    ../../../../../../code/calendar/unix/timestamp.h \
    ../../../../../../code/calendar/_ent/weekday.h \
    ../../../../../../code/calendar/_ent/weekdays.h \
    ../../../../../../code/calendar/_ent/year.h \
    ../../../../../../code/calendar/_ent/years.h

SOURCES += \
  ../../../../../../code/calendar/_ent/day.cpp \
  ../../../../../../code/calendar/_ent/hour.cpp \
  ../../../../../../code/calendar/_ent/minute.cpp \
  ../../../../../../code/calendar/_ent/month.cpp \
  ../../../../../../code/calendar/_ent/second.cpp \
  ../../../../../../code/calendar/unix/internal/timestamp.cpp \
  ../../../../../../code/calendar/_ent/weekday.cpp \
  ../../../../../../code/calendar/_tst/ent000/main.cpp

