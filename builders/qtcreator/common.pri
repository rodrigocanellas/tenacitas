#CONFIG(debug, debug|release) {
#    build_type=debug
#    message("This is debug mode or debug_and_release")
#} else {
#    build_type=release
#    message("This is release mode or debug_and_release")
#}

QMAKE_CXXFLAGS += -std=c++11



#spec = $$basename(QMAKESPEC)

#equals(spec,"") {
#    spec="spec_undefined"
#}


#base_dir=$$PWD/../..
#code_dir=$$base_dir/code
#builder_dir=$$base_dir/builders/qtcreator
#products_dir=$$base_dir/products
#third_dir=$$base_dir/3rd
#tmp_dir=$$products_dir/tmp/qtcreator/$$spec/$$build_type/$$TARGET
#libs_dir=$$products_dir/$$spec/$$build_type/library
#bins_dir=$$products_dir/$$spec/$$build_type/binary
#test_dir=$$products_dir/$$spec/$$build_type/test
#example_dir=$$products_dir/$$spec/$$build_type/example

base_dir=$$PWD/../..
code_dir=$$base_dir/code
builder_dir=$$base_dir/builders/qtcreator
products_dir=$$base_dir/products
third_dir=$$base_dir/3rd
tmp_dir=$$products_dir/tmp/qtcreator/$$TARGET
libs_dir=$$products_dir/library
bins_dir=$$products_dir/binary
test_dir=$$products_dir/test
example_dir=$$products_dir/example


OUT_PWD = $${tmp_dir}
MOC_DIR += $${tmp_dir}/moc
UI_DIR  += $${tmp_dir}/ui
#UI_DIR  += $$_PRO_FILE_PWD_/../..
OBJECTS_DIR += $${tmp_dir}/obj
RCC_DIR += $${tmp_dir}/rsc

QMAKE_CXXFLAGS += -std=c++11 -Werror
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

#equals(TEMPLATE,dll) {
#    DESTDIR = $$libs_dir
#}

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
    static_lib_ext=lib
}

INCLUDEPATH += $$code_dir
INCLUDEPATH += $$UI_DIR
INCLUDEPATH += $${third_dir}/include
#INCLUDEPATH += $$_PRO_FILE_PWD_/../../mhi

message("_PRO_FILE_PWD_ = "$$_PRO_FILE_PWD_)
message("build_type ="$$build_type)
message("base_dir= "$$base_dir)
message("code_dir= "$$code_dir)
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
