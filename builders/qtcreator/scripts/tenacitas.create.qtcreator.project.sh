#!/bin/sh

# check params
if [ ! $# -eq 2 ]; then
    echo "Syntax: `basename $0` <base-dir> <project-name>"
    return 10
fi

# set vars
base_dir="$1"
prj_name="$2"


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

# create dirs
src_code_dir="$base_dir/src/$prj_name"
mkdir -p "$src_code_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$src_code_dir'"
    return 40
fi

tst_code_dir="$base_dir/tst/$prj_name"
mkdir -p "$tst_code_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$tst_code_dir'"
    return 50
fi

doc_dir="$base_dir/doc/$prj_name"
mkdir -p "$doc_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$doc_dir'"
    return 60
fi

src_build_dir="$base_dir/builders/qtcreator/projects/$prj_name"
mkdir -p "$src_build_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$src_build_dir'"
    return 70
fi

tst_build_dir="$base_dir/builders/qtcreator/projects/$prj_name/tst"
mkdir -p "$tst_build_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$tst_code_dir'"
    return 80
fi


# create dep dir for tst
dep_name="dep"
dep_build_dir="$tst_build_dir/$dep_name"
mkdir -p "$dep_build_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$dep_build_dir'"
    return 90
fi

# create prj .pro file
src_build_file="$src_build_dir/$prj_name".pro
touch "$src_build_file"
echo "TEMPLATE = subdirs" >> "$src_build_file"
echo ""                   >> "$src_build_file"
echo "SUBDIRS += \\"      >> "$src_build_file"
echo "  tst"              >> "$src_build_file"
echo ""                   >> "$src_build_file" 
echo "CONFIG += ordered"  >> "$src_build_file"

# create tst .pro file
tst_build_file="$tst_build_dir/tst.pro"
touch "$tst_build_file"
echo "TEMPLATE = subdirs" >> "$tst_build_file"
echo ""                   >> "$tst_build_file"
echo "SUBDIRS += \\"      >> "$tst_build_file"
echo ""  "$dep_name"      >> "$tst_build_file"
echo ""                   >> "$tst_build_file"
echo "CONFIG += ordered"  >> "$tst_build_file"

# create dep .pro file
dep_build_file="$dep_build_dir/$dep_name.pro"
touch "$dep_build_file"
echo "TEMPLATE = subdirs"                      >> "$dep_build_file"
echo ""                                        >> "$dep_build_file"
echo "include(../../../../common.pri)"         >> "$dep_build_file"
echo ""                                        >> "$dep_build_file"
echo "SUBDIRS += \\"                           >> "$dep_build_file"
echo "  \$\$builder_dir/tester/tester.headers" >> "$dep_build_file"


return 0
