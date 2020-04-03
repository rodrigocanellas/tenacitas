TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.calendar._tst.weekly_repetiotion_000


include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
    ../../../../../../code/calendar/_ent/day.cpp \
    ../../../../../../code/calendar/_ent/hour.cpp \
    ../../../../../../code/calendar/_ent/minute.cpp \
    ../../../../../../code/calendar/_ent/month.cpp \
    ../../../../../../code/calendar/_ent/second.cpp \
    ../../../../../../code/calendar/unix/internal/timestamp.cpp \
    ../../../../../../code/calendar/_ent/weekday.cpp \
    ../../../../../../code/calendar/_tst/weekly_repetition_000/main.cpp


