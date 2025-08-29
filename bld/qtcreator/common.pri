
QMAKE_CXXFLAGS += -DTENACITAS_LOG
# DEFINES+= -DTENACITAS_LOG

CONFIG += sdk_no_version_check

CONFIG(debug, debug|release) {
    cfg=debug
}


CONFIG(release, debug|release) {
    cfg=release
}


arch=32
contains(QMAKE_HOST.arch, x86_64):{
    arch=64
}

macx:os=mac
linux:os=linux
win32:os=win32


BASE_DIR=$$PWD/../../src


products_dir=$$BASE_DIR/../../prd/$$os-$$cfg-$$arch
third_dir=$$BASE_DIR/3rd
libs_dir=$$products_dir/lib
bins_dir=$$products_dir/app
test_dir=$$products_dir/tst
exp_dir=$$products_dir/exp
poc_dir=$$products_dir/poc
plgs_dir=$$products_dir/plg
tmp_dir=$$products_dir/.tmp/$$TARGET

OUT_PWD = $${tmp_dir}
MOC_DIR += $${tmp_dir}/moc
UI_DIR  += $${tmp_dir}/ui
#UI_DIR  += $$_PRO_FILE_PWD_/../..
OBJECTS_DIR += $${tmp_dir}/obj
#RCC_DIR += $${tmp_dir}/rsc

QMAKE_CXX = g++-13
CONFIG += c++23



equals(TEMPLATE,app) {
    DESTDIR = $$bins_dir
    contains(CONFIG,test) {
        DESTDIR=$$test_dir
    }
    contains(CONFIG,example){
        DESTDIR = $$exp_dir
    }
    contains(CONFIG,poc){
        DESTDIR = $$poc_dir
    }

}

equals(TEMPLATE,lib) {
    contains(CONFIG,plugin) {
        DESTDIR = $$plgs_dir
        CONFIG += dll
    }
    ! contains(CONFIG,plugin) {
      DESTDIR = $$libs_dir
    }
}


LIBS += -L$$libs_dir -L$${third_dir}/lib


unix {
    static_lib_ext=a
    QMAKE_CXXFLAGS+=-DPOSIX
    QMAKE_CXXFLAGS += -Werror
    QMAKE_CXXFLAGS += -std=c++20        
    QMAKE_LFLAGS+=-fPIC -Wl,-rpath,$$libs_dir
    LIBS += -L$${third_dir}/lib/$$arch/unix
    LIBS += -lpthread
}

QMAKE_CXXFLAGS+=-fconcepts-diagnostics-depth=3



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

    LIBS += -L$${third_dir}/lib/windows/$$arch
}


INCLUDEPATH += $$UI_DIR $$BASE_DIR $$third_dir/websocket $$third_dir/asio/asio/include


message("_PRO_FILE_PWD_ = "$$_PRO_FILE_PWD_)
message("BASE_DIR= "$$BASE_DIR)
message("build_type ="$$build_type)
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
message("tst_dir = "$$test_dir)
message("QMAKE_CXXFLAGS = "$$QMAKE_CXXFLAGS)
message("CONFIG = "$$CONFIG)
message("QMAKESPEC = "$$QMAKESPEC)
message("static_lib_ext = "$$static_lib_ext)
message("third_dir = "$$third_dir)

#message("LIBS = "$$LIBS)

# =============================================================================
