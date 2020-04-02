TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.calendar.tst.weekly_repetiotion_000


include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
    ../../../../../../code/calendar/ent/day.cpp \
    ../../../../../../code/calendar/ent/hour.cpp \
    ../../../../../../code/calendar/ent/minute.cpp \
    ../../../../../../code/calendar/ent/month.cpp \
    ../../../../../../code/calendar/ent/second.cpp \
    ../../../../../../code/calendar/unix/internal/timestamp.cpp \
    ../../../../../../code/calendar/ent/weekday.cpp \
    ../../../../../../code/calendar/tst/weekly_repetition_000/main.cpp


