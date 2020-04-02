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
tst_group_name="$prj_name.tst"
tst_group_dir="$prj_dir/$tst_group_name"

tst_dir="$tst_group_dir/$prj_name.tst.$tst_name"
tst_file="$tst_dir/$prj_name.tst.$tst_name.pro"

mkdir -p "$tst_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$tst_dir'"
    return 45
fi


# create tst .pro file
touch "$tst_file"
echo "TEMPLATE = app"                             >>  "$tst_file"
echo ""                                           >>  "$tst_file"
echo "CONFIG -= qt"                               >>  "$tst_file"
echo ""                                           >>  "$tst_file"
echo "CONFIG += test"                             >>  "$tst_file"
echo ""                                           >>  "$tst_file"
echo "TARGET = tenacitas.$prj_name.tst.$tst_name" >>  "$tst_file"
echo ""                                           >>  "$tst_file" 
echo "include (../../../../common.pri)"           >>  "$tst_file"





