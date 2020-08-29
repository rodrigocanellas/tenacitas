
QMAKE_CXXFLAGS += -std=c++11 -Werror
QMAKE_LFLAGS+=-fPIC -Wl,-rpath,$$libs_dir

base_dir=$$PWD/../..
code_src_dir=$$base_dir/src
test_src_dir=$$base_dir/tst
builder_dir=$$base_dir/builders/qtcreator/projects
products_dir=$$base_dir/prd
third_dir=$$base_dir/3rd

libs_dir=$$products_dir/lib
bins_dir=$$products_dir/bin
test_dir=$$products_dir/tst
example_dir=$$products_dir/example

unix {
    tmp_dir=~/tenacitas/tmp/qtcreator/$$TARGET
}

win32 {
    tmp_dir=$$TMP/tenacitas/tmp/qtcreator/$$TARGET
}

OUT_PWD = $${tmp_dir}
MOC_DIR += $${tmp_dir}/moc
UI_DIR  += $${tmp_dir}/ui
#UI_DIR  += $$_PRO_FILE_PWD_/../..
OBJECTS_DIR += $${tmp_dir}/obj
RCC_DIR += $${tmp_dir}/rsc


CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Werror
QMAKE_LFLAGS+=-fPIC -Wl,-rpath,$$libs_dir

equals(TEMPLATE,app) {
    DESTDIR = $$bins_dir
    contains(CONFIG,test) {
        DESTDIR=$$test_dir
    }
    contains(CONFIG,example){
        DESTDIR = $$example_dir
    }
}

equals(TEMPLATE,lib) {
    DESTDIR = $$libs_dir
    ! contains(CONFIG,staticlib) {
      CONFIG += shared_and_static
    }
}

LIBS += -L$$libs_dir -L$${third_dir}/lib
LIBS += -lpthread

unix {
    static_lib_ext=a
}

win32 {
    equals(QMAKE_CXX,mingw) {
        static_lib_ext=a
    }
    equals(QMAKE_CXX,g++) {
        static_lib_ext=a
    }
    equals(QMAKE_CXX,msvc) {
        static_lib_ext=lib
    }
}

INCLUDEPATH += $$code_src_dir
INCLUDEPATH += $$test_src_dir
INCLUDEPATH += $$UI_DIR
INCLUDEPATH += $${third_dir}/include
#INCLUDEPATH += $$_PRO_FILE_PWD_/../../mhi

message("_PRO_FILE_PWD_ = "$$_PRO_FILE_PWD_)
message("build_type ="$$build_type)
message("base_dir= "$$base_dir)
message("code_src_dir= "$$code_src_dir)
message("products_dir = "$$products_dir)
message("TARGET = "$$TARGET)
message("SPEC = "$$spec)
message("INCLUDEPATH = "$$INCLUDEPATH)
message("OBJECTS_DIR = "$$OBJECTS_DIR)
message("DESTDIR = "$$DESTDIR)
message("FORMS = "$$FORMS)
message("SOURCES = "$$SOURCES)
message("HEADERS= "$$HEADERS)
message("UI_DIR  = "$$UI_DIR)
message("MOC_DIR = "$$MOC_DIR)
message("OUT_PWD = "$$OUT_PWD)
message("libs_dir = "$$libs_dir)
message("bins_dir = "$$bins_dir)
message("QMAKE_CXXFLAGS = "$$QMAKE_CXXFLAGS)
message("CONFIG = "$$CONFIG)
message("QMAKESPEC = "$$QMAKESPEC)
message("static_lib_ext = "$$static_lib_ext)



#message("LIBS = "$$LIBS)




# =============================================================================
