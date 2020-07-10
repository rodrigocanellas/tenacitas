CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.measures.time

include (../../../common.pri)

HEADERS+=$$code_src_dir/measures/internal/amount_t.h \
         $$code_src_dir/measures/internal/convert_t.h \
         $$code_src_dir/measures/time_amounts.h  \
         $$code_src_dir/measures/day.h  \
         $$code_src_dir/measures/hour.h  \
         $$code_src_dir/measures/millisecond.h  \
         $$code_src_dir/measures/minute.h  \
         $$code_src_dir/measures/month.h  \
         $$code_src_dir/measures/second.h  \
         $$code_src_dir/measures/weekday.h  \
         $$code_src_dir/measures/week.h  \
         $$code_src_dir/measures/year.h

SOURCES += $$code_src_dir/measures/internal/day.cpp  \
           $$code_src_dir/measures/internal/hour.cpp  \
           $$code_src_dir/measures/internal/minute.cpp  \
           $$code_src_dir/measures/internal/month.cpp  \
           $$code_src_dir/measures/internal/second.cpp  \
           $$code_src_dir/measures/internal/weekday.cpp


