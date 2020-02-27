#! /bin/sh

# Displays an explanation on how to use this program
syntax() {
    echo "Syntax: `basename $0` <base-dir> <mode> <params>"    
    echo ""
    echo "Where <base-dir> is the base directory of development, like '/home/user000/tenacitas."
    echo "Below this directory must be 'code' and 'builders/qtcreator' directories."
    echo ""
    echo "If <mode> = 'prj', then <params> must be: "
    echo "\t<prj-name>"
    echo ""
    echo "If <mode> = 'cmp', then <params> must be:"
    echo "\t<prj-name> <component-type>, where <component-type> may be  'bus', 'ent', 'per', 'mhi' or 'com'"
    echo ""
    echo "If <mode> = 'prd', then <params> must be:"
    echo "\t<prj-name> <product-name> <product-type>, where <product-type> may be  'lib' or 'pgm'"
    echo ""
    echo "If <mode> = 'dep', then <params> must be:"
    echo "\t<prj-name> <prj-file>, where <prj-file> is the '.pro' file name of the project that this project depends on"
    echo ""
    echo "If <mode> = 'tst' then <params> must be:"
    echo "\t<prj-name> <test-name> "
}

# Checks if necessary directories exist
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

# Creates directories and files for a new project
create_prj() {
    local base_dir="$1"
    local code_dir="$1/code"
    local builder_dir="$1/builders/qtcreator/projects"
    local prj_name="$2"

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

create_cmp() {
    local base_dir="$1"
    local code_dir="$1/code"
    local builder_dir="$1/builders/qtcreator/projects"
    local prj_name="$2"
    local cmp_type="$3"
    local prj_file="$builder_dir/$prj_name/$prj_name.pro"

    if [ ! -d "$builder_dir/$prj_name" ]; then
	create_prj "$base_dir" "$prj_name"
	if [ $? -ne 0 ]; then
	    echo "ERROR creating project '$prj_name'"
	    return 1
	fi
    fi

    if [ "$cmp_type" != "ent" -a "$cmp_type" != "bus" -a "$cmp_type" != "per" -a "$cmp_type" != "com" -a "$cmp_type" != "mhi" ]; then
	echo "ERROR '$cmp_type' is not a valid component type"
	return 2
    fi

    subdirs_found=0
    cat $prj_file | while read line; do
	if [ 		  
	
    done
    

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
    create_prj "$1" "$3"
else
    if [ "$2" = "cmp" ]; then
	create_cmp "$1" "$3" "$4"
    else
	echo "mode '$2' not yet implemented"
    fi
fi
	


