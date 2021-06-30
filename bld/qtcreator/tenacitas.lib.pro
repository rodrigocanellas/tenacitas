



include (tenacitas.lib.pri)

message("TENACITAS = "$$TENACITAS)

TEMPLATE = subdirs

HEADERS += \
    $$src_code_dir/tenacitas.lib/type.h \
    $$src_code_dir/tenacitas.lib/number.h  \
    $$src_code_dir/tenacitas.lib/program.h  \
    $$src_code_dir/tenacitas.lib/calendar.h \
    $$src_code_dir/tenacitas.lib/tester.h  \
    $$src_code_dir/tenacitas.lib/logger.h  \
    $$src_code_dir/tenacitas.lib/event.h


SUBDIRS += \
  exp \
  tst

