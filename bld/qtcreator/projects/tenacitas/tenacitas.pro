include (../../common.pri)

TEMPLATE = subdirs

HEADERS += \
    $$code_src_dir/tenacitas/type.h \
    $$code_src_dir/tenacitas/number.h  \
    $$code_src_dir/tenacitas/program.h  \
    $$code_src_dir/tenacitas/calendar.h \
    $$code_src_dir/tenacitas/tester.h  \
    $$code_src_dir/tenacitas/logger.h  \
    $$code_src_dir/tenacitas/async.h \
    $$code_src_dir/tenacitas/macros.h \
    $$code_src_dir/tenacitas/message.h \


SUBDIRS += \
  tst \
  exp
