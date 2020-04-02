#!/bin/sh

# check params
if [ ! $# -eq 3 ]; then
    echo "Syntax: `basename $0` <base-dir> <project-name> <product-name>"
    return 10
fi

# set vars
base_dir="$1"
prj_name="$2"
prod_name="$3"

# check if base_dir exists
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
    echo "'$prj_name' does not exists"
    return 40
fi

# check if prod dir exists
prod_dir="$prj_dir/$prj_name.$prod_name"
if [ -d "$prod_dir" ]; then
    echo "'$prod_name' exists"
    return 45
fi


# create prod dir
mkdir -p "$prod_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$prod_dir'"
    return 50
fi

# create prod .pro file
prod_file="$prod_dir/$prj_name.$prod_name.pro"
touch "$prod_file"
echo "CONFIG -= qt"                            >> "$prod_file"
echo ""                                        >> "$prod_file"
echo "TEMPLATE = lib"                          >> "$prod_file"
echo ""                                        >> "$prod_file"
echo "CONFIG += staticlib"                     >> "$prod_file"
echo ""                                        >> "$prod_file"
echo "TARGET = tenacitas.$prj_name.$prod_name" >> "$prod_file"
echo ""                                        >> "$prod_file"
echo ""                                        >> "$prod_file"
echo "include (../../../common.pri)"           >> "$prod_file"






