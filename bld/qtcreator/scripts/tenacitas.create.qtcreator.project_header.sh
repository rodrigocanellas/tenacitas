#!/bin/sh

# check params
if [ ! $# -eq 3 ]; then
    echo "Syntax: `basename $0` <base-dir> <project-name> <header-name>"
    return 10
fi

# set vars
base_dir="$1"
prj_name="$2"
header_name="$3"

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

src_dir=$base_dir/src/$prj_name

# create header
header_file="$src_dir/$header_name.h"
if [ -f "$header_file" ]; then
   echo "$header_file already exists"
   return 60
fi

PRJ_NAME=`echo "$prj_name" | tr a-z A-Z`
HEADER_NAME=`echo "$header_name" | tr a-z A-Z`
line=TENACITAS_"$PRJ_NAME"_"$HEADER_NAME"_H
echo "#ifndef $line"                                                                      >> "$header_file"
echo "#define $line"                                                                      >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo "/// \\\copyright This file is under GPL 3 license. Please read the \p LICENSE file" >> "$header_file"
echo "/// at the root of \p tenacitas directory"                                          >> "$header_file"   
echo ""                                                                                   >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo "/// \\\author Rodrigo Canellas - rodrigo.canellas@gmail.com"                        >> "$header_file"
echo "/// \\date `date +\"%m/%Y\"`"                                                       >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo "/// \\\brief namespace of the organization"                                         >> "$header_file"
echo "namespace tenacitas {"                                                              >> "$header_file"
echo "/// \\\brief namespace of the project"                                              >> "$header_file"
echo "namespace $prj_name {"                                                              >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo "} // namespace $prj_name"                                                           >> "$header_file"
echo "} // namespace tenacitas"                                                           >> "$header_file"
echo ""                                                                                   >> "$header_file"
echo "#endif"                                                                             >> "$header_file"

return 0
