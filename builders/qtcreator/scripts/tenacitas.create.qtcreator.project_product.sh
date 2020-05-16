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
    path_to_scripts=`dirname $0`
    $path_to_scripts/tenacitas.create.qtcreator.project.sh $base_dir $prj_name 2> /dev/null
    if [ $? -ne 0 ]; then
	echo "error creating project $prj_name"
	return 40
    fi
fi

# check if prod dir exists
prod_build_dir="$prj_dir/$prj_name.$prod_name"
if [ -d "$prod_build_dir" ]; then
    echo "'$prod_name' exists"
    return 45
fi


# create prod dir
mkdir -p "$prod_build_dir" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$prod_build_dir'"
    return 50
fi


# create build prod .pro file
prod_build_file="$prod_build_dir/$prj_name.$prod_name.pro"
touch "$prod_build_file"
echo "CONFIG -= qt"                            >> "$prod_build_file"
echo ""                                        >> "$prod_build_file"
echo "TEMPLATE = lib"                          >> "$prod_build_file"
echo ""                                        >> "$prod_build_file"
echo "CONFIG += staticlib"                     >> "$prod_build_file"
echo ""                                        >> "$prod_build_file"
echo "TARGET = tenacitas.$prj_name.$prod_name" >> "$prod_build_file"
echo ""                                        >> "$prod_build_file"
echo "include (../../../common.pri)"           >> "$prod_build_file"
echo ""                                        >> "$prod_build_file"

# create code prod dir
prod_src_dir="$base_dir/src/$prj_dir/$prod_name"
mkdir -p "$prod_src_dir"  2> /dev/null
if [ $? -ne 0 ]; then
    echo "error creating '$prod_src_dir'"
    return 60
fi

















