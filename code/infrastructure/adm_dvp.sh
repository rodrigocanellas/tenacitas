#! /bin/sh

syntax() {
    echo "Syntax: `basename $0` <base-dir> <mode> <params>"    
    echo ""
    echo "Where <base-dir> is the base directory of development, like '/home/user000/tenacitas."
    echo "Below this directory must be 'code' and 'builders/qtcreator' directories."
    echo ""
    echo "If <mode> = 'prj', then <params> must be: "
    echo "\t<prj-name> <prj-type>, where <prj-type> is 'pgm' or 'lib'"
    echo ""
    echo "If <mode> = 'lay', then <params> must be:"
    echo "\t<prj-name> <layer-type>, where <layer-type> is 'bus', 'ent', 'per', 'mhi'"
    echo ""
    echo "If <mode> = 'tst' then <params> must be:"
    echo "\t<prj-name> <layer-type> <test-name> "
}

check_base_dir() {
    local base_dir="$1"
    if [ ! -d "$base_dir" ]; then
	echo "$base_dir not found"
	return 1
    fi	
    if [ ! -d "$base_dir/code" ]; then
	echo "$base_dir/code not found"
	return 2
    fi
    if [ ! -d "$base_dir/builders/qtcreator" ]; then
	echo "$base_dir/builders/qtcreator not found"
	return 2
    fi
}

create_prj() {
    local base_dir="$1"
    local code_dir="$1/code"
    local builder_dir="$1/builders/qtcreator/projects"
    local prj_name="$3"

    # ##### creating directories 
    echo "Creating project '$prj_name' in '$base_dir'"
    if [ ! -d "$code_dir/$prj_name" ]; then
	mkdir -p "$code_dir/$prj_name" 2> /dev/null
	if [ $? -ne 0 ]; then
	    echo "Error creating '$code_dir/$prj_name'"
	    return 1
	fi
    fi
    if [ ! -d "$builder_dir/$prj_name" ]; then
	mkdir -p "$builder_dir/$prj_name" 2> /dev/null
	if [ $? -ne 0 ]; then
	    echo "Error creating '$builder_dir/$prj_name'"
	    return 2
	fi
    fi

    proj_file_name="$builder_dir/$prj_name/$prj_name.pro"
    touch "$proj_file_name"
    echo "SUBDIRS = " >> "$proj_file_name"
    echo ""  >> "$proj_file_name"
    echo "TEMPLATE = subdirs"  >> "$proj_file_name"
    echo ""  >> "$proj_file_name"
    echo "CONFIG += ordered" >> "$proj_file_name"
}    

# *********************** start

if [ $# -lt 1 ]; then
    syntax
    return 1
fi

check_base_dir "$1"
if [ $? -ne 0 ]; then
    syntax
    return 1
fi

if [ "$2" = "prj" ]; then
    if [ $# -ne 3 ]; then
	syntax
	return 1
    fi
    create_prj "$1" "$2" "$3"
else
    echo "Mode '$2' not yet implemented"
fi
	


