#!/bin/sh

# check params
if [ ! $# -eq 4 ]; then
    echo "Syntax: `basename $0` <base-dir> <project-name> <layer-name> <header-name>"
    return 10
fi

# set vars
base_dir="$1"
prj_name="$2"
layer_name="$3"
header_name="$4"

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



# create header
header_file="$code_layer_dir/$header_name.h"
if [ -f "$header_file" ]; then
   echo "$header_file already exists"
   return 60
fi
echo "header_file = $header_file"
PRJ_NAME=`echo "$prj_name" | tr a-z A-Z`
LAYER_NAME=`echo "$layer_name" | tr a-z A-Z` 
HEADER_NAME=`echo "$header_name" | tr a-z A-Z`
line=TENACITAS_"$PRJ_NAME"_"$LAYER_NAME"_"$HEADER_NAME"_H
echo "#ifndef $line"                                               >> "$header_file"
echo "#define $line"                                               >> "$header_file"
echo ""                                                            >> "$header_file"
echo ""                                                            >> "$header_file"
echo "/// \\\author Rodrigo Canellas - rodrigo.canellas@gmail.com" >> "$header_file"
echo "/// \\date `date +\"%m/%Y\"`"                                >> "$header_file"
echo ""                                                            >> "$header_file"
echo ""                                                            >> "$header_file"
echo "namespace tenacitas {"                                       >> "$header_file"
echo "namespace $prj_name {"                                       >> "$header_file"
echo "namespace $layer_name {"                                     >> "$header_file"
echo ""                                                            >> "$header_file"
echo ""                                                            >> "$header_file"
echo "} // namespace $layer_name"                                  >> "$header_file"
echo "} // namespace $prj_name"                                    >> "$header_file"
echo "} // namespace tenacitas"                                    >> "$header_file"
echo ""                                                            >> "$header_file"
echo "#endif"                                                      >> "$header_file"

