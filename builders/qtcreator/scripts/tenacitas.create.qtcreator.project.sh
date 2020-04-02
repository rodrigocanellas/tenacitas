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

# check if prj dir exists
prj_dir="$qt_dir/$prj_name"
if [ -d "$prj_dir" ]; then
    echo "'$prj_name' already exists"
    return 40
fi

# create prj dir
mkdir -p "$prj_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$prj_name'"
    return 50
fi

# create tst dir
tst_name="$prj_name.tst"
tst_dir="$prj_dir/$tst_name"
mkdir -p "$tst_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$tst_dir'"
    rm -rf "$prj_dir" 2> /dev/null
    return 60
fi

# create dep dir for tst
dep_name="$tst_name.dep"
dep_dir="$tst_dir/$dep_name"
mkdir -p "$dep_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$dep_dir'"
    rm -rf "$prj_dir" 2> /dev/null
    return 70
fi

# create prj .pro file
prj_file="$prj_dir/$prj_name".pro
touch "$prj_file"
echo "TEMPLATE = subdirs" >> "$prj_file"
echo ""                   >> "$prj_file"
echo "SUBDIRS += \\"      >> "$prj_file"
echo "  $prj_name.tst"    >> "$prj_file"
echo ""                   >> "$prj_file" 
echo "CONFIG += ordered"  >> "$prj_file"

# create tst .pro file
tst_file="$tst_dir/$tst_name.pro"
touch "$tst_file"
echo "TEMPLATE = subdirs" >> "$tst_file"
echo ""                   >> "$tst_file"
echo "SUBDIRS += \\"      >> "$tst_file"
echo ""  "$dep_name"      >> "$tst_file"
echo ""                   >> "$tst_file"
echo "CONFIG += ordered"  >> "$tst_file"

# create dep .pro file
dep_file="$dep_dir/$dep_name.pro"
touch "$dep_file"
echo "TEMPLATE = subdirs"                            >> "$dep_file"
echo ""                                              >> "$dep_file"
echo "SUBDIRS += \\"                                 >> "$dep_file"
echo "  ../../../tester/headers/tester_headers.pro"  >> "$dep_file"



