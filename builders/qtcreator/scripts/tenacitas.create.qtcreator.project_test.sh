#!/bin/sh

# check params
if [ ! $# -eq 3 ]; then
    echo "Syntax: `basename $0` <base-dir> <project-name> <test-name>"
    return 10
fi

# set vars
base_dir="$1"
prj_name="$2"
tst_name="$3"

# check if base dir exists
if [ ! -d "$base_dir" ]; then
    echo "'$base_dir' does not exist"
    return 20
fi

# check if qt dir exists
qt_dir="$base_dir"/builders/qtcreator/projects
if [ ! -d "$qt_dir" ]; then
    echo "'$qt_dir' does not exist"
    return 30
fi

# check if prj dir exists
prj_dir="$qt_dir/$prj_name"
if [ ! -d "$prj_dir" ]; then
    echo "'$prj_name' does not exist"
    return 40
fi

# create tst 
tst_group_name="_tst"
tst_group_dir="$prj_dir/$tst_group_name"

tst_dir="$tst_group_dir/$tst_name"
tst_file="$tst_dir/$tst_name.pro"

mkdir -p "$tst_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$tst_dir'"
    return 45
fi

# create main.cpp file
main_dir="$base_dir/code/$prj_name/_tst/$tst_name"
main_file="$main_dir/main.cpp"
mkdir -p $main_dir 2> /dev/null
touch $main_file
echo "#include <logger/cerr/log.h>"                          >> "$main_file"
echo "#include <tester/_bus/test.h>"                         >> "$main_file"
echo ""                                                      >> "$main_file"
echo "using namespace tenacitas;"                            >> "$main_file"
echo ""                                                      >> "$main_file"
echo "struct $tst_name { "                                   >> "$main_file"
echo "  bool operator()() { "                                >> "$main_file"
echo ""                                                      >> "$main_file"
echo "    return true;"                                      >> "$main_file"
echo "  }"                                                   >> "$main_file"
echo ""                                                      >> "$main_file"
echo "  static std::string desc() { return \"\"; }"          >> "$main_file"
echo ""                                                      >> "$main_file"
echo "  static std::string name() { return \"$tst_name\"; }" >> "$main_file"
echo ""                                                      >> "$main_file"
echo "};"                                                    >> "$main_file"
echo ""                                                      >> "$main_file"
echo "int main(int argc, char **argv) {"                     >> "$main_file"
echo "  logger::cerr::log::set_debug(); "                    >> "$main_file"
echo "  tester::_bus::test::run<$tst_name>(argc, argv);  "   >> "$main_file"
echo "}"                                                     >> "$main_file"
echo ""                                                      >> "$main_file"

# create tst .pro file
touch "$tst_file"
echo "TEMPLATE = app"                                                   >> "$tst_file"
echo ""                                                                 >> "$tst_file"
echo "CONFIG -= qt"                                                     >> "$tst_file"
echo ""                                                                 >> "$tst_file"
echo "CONFIG += test"                                                   >> "$tst_file"
echo ""                                                                 >> "$tst_file"
echo "TARGET = tenacitas.$prj_name._tst.$tst_name"                      >> "$tst_file"
echo ""                                                                 >> "$tst_file"
echo "SOURCES += \\"                                                    >> "$tst_file"
echo "  ../../../../../../code/$prj_name/_tst/$tst_name/main.cpp"       >> "$tst_file"
echo ""                                                                 >> "$tst_file"
echo "include (../../../../common.pri)"                                 >> "$tst_file"
echo ""                                                                 >> "$tst_file"
echo "LIBS += \$\$libs_dir/libtenacitas.logger.cerr.\$\$static_lib_ext" >> "$tst_file"





