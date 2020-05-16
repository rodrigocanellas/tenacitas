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
    path_to_scripts=`dirname $0`
    $path_to_scripts/tenacitas.create.qtcreator.project.sh $base_dir $prj_name 2> /dev/null
    if [ $? -ne 0 ]; then
	echo "error creating project $prj_name"
	return 40
    fi
fi

# create tst
tst_code_dir="$base_dir/tst/$prj_name/$tst_name"
tst_main_file="$tst_code_dir/main.cpp"
tst_build_dir="$base_dir/builders/qtcreator/projects/$prj_name/tst/$tst_name"
tst_build_file="$tst_build_dir/$tst_name.pro"

mkdir -p "$tst_build_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$tst_build_dir'"
    return 45
fi

# create main.cpp file

mkdir -p $tst_code_dir 2> /dev/null
touch $tst_main_file
echo ""                                                      >> "$tst_main_file"
echo "#include <cstdint>"                                    >> "$tst_main_file"
echo "#include <iostream>"                                   >> "$tst_main_file"
echo "#include <string>"                                     >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "#include <tester/test.h>"                              >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "using namespace tenacitas;"                            >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "struct $tst_name { "                                   >> "$tst_main_file"
echo "  bool operator()() { "                                >> "$tst_main_file"
echo "    // insert your test code here"                     >> "$tst_main_file"
echo "    return true;"                                      >> "$tst_main_file"
echo "  }"                                                   >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "  static std::string desc() { return \"\"; }"          >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "  static std::string name() { return \"$tst_name\"; }" >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "};"                                                    >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"
echo "int main(int argc, char **argv) {"                     >> "$tst_main_file"
echo "  tester::test::run<$tst_name>(argc, argv);  "         >> "$tst_main_file"
echo "}"                                                     >> "$tst_main_file"
echo ""                                                      >> "$tst_main_file"

# create tst .pro file
touch "$tst_build_file"
echo "TEMPLATE = app"                                  >> "$tst_build_file"
echo ""                                                >> "$tst_build_file"
echo "CONFIG -= qt"                                    >> "$tst_build_file"
echo ""                                                >> "$tst_build_file"
echo "CONFIG += test"                                  >> "$tst_build_file"
echo ""                                                >> "$tst_build_file"
echo "TARGET = tenacitas.$prj_name.tst.$tst_name"      >> "$tst_build_file"
echo ""                                                >> "$tst_build_file"
echo "include (../../../../common.pri)"                >> "$tst_build_file"
echo ""                                                >> "$tst_build_file"
echo "SOURCES += \\"                                   >> "$tst_build_file"
echo "  \$\$test_src_dir/$prj_name/$tst_name/main.cpp" >> "$tst_build_file"
echo ""                                                >> "$tst_build_file"





