CONFIG -= qt

TEMPLATE = lib

CONFIG += staticlib

TARGET = tenacitas.measures.time

include (../../../common.pri)

HEADERS+=$$code_src_dir/measures/internal/amount_t.h \
         $$code_src_dir/measures/internal/convert_t.h \
         $$code_src_dir/measures/time/amounts.h  \
         $$code_src_dir/measures/time/day.h  \
         $$code_src_dir/measures/time/hour.h  \
         $$code_src_dir/measures/time/millisecond.h  \
         $$code_src_dir/measures/time/minute.h  \
         $$code_src_dir/measures/time/month.h  \
         $$code_src_dir/measures/time/second.h  \
         $$code_src_dir/measures/time/weekday.h  \
         $$code_src_dir/measures/time/week.h  \
         $$code_src_dir/measures/time/year.h

SOURCES += $$code_src_dir/measures/time/internal/day.cpp  \
           $$code_src_dir/measures/time/internal/hour.cpp  \
           $$code_src_dir/measures/time/internal/minute.cpp  \
           $$code_src_dir/measures/time/internal/month.cpp  \
           $$code_src_dir/measures/time/internal/second.cpp  \
           $$code_src_dir/measures/time/internal/weekday.cpp


