TEMPLATE = app
CONFIG -= qt
CONFIG+=test

TARGET = tenacitas.calendar._tst.weekly_repetiotion_000


include (../../../../common.pri)


LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext

SOURCES += \
    $$code_src_dir/calendar/_ent/day.cpp \
    $$code_src_dir/calendar/_ent/hour.cpp \
    $$code_src_dir/calendar/_ent/minute.cpp \
    $$code_src_dir/calendar/_ent/month.cpp \
    $$code_src_dir/calendar/_ent/second.cpp \
    $$code_src_dir/calendar/unix/internal/timestamp.cpp \
    $$code_src_dir/calendar/_ent/weekday.cpp \
    $$code_src_dir/calendar/_tst/weekly_repetition_000/main.cpp


