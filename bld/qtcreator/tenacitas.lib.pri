include (../../../tenacitas.builder/tenacitas.builder.pri)

src_code_dir = $$PWD/../../src
tst_code_dir = $$PWD/../..
exp_code_dir = $$PWD/../..


INCLUDEPATH += $$tst_code_dir
INCLUDEPATH += $$exp_code_dir


message("src_code_dir = "$$src_code_dir)
message("tst_code_dir = "$$tst_code_dir)
message("exp_code_dir = "$$exp_code_dir)
