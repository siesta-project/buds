#!/bin/bash

# Requirements:
#  GNU sed compilance (q-specifier)

# Author: Nick R. Papior, 2016

# This small script ensures a custom setting of
# variables for consistent editing of files across
# platforms and user-boxes.
#
# Essentially the script sets the design choices
# regarding the indentation and other settings.

# Each file has the settings at the end of the file
# The settings MUST be the last lines in each file.

_LINE_END="refype -- local file settings"

# Here are the fortran local variables
declare -A fv
for v in do if structure continuation associate \
			 critical program type
do
    fv["f90-$v-indent"]=2
done

function update_setting() {
    # Set settings in this file:
    local f=$1
    shift
    local ftype=fortran

    function clean_tmp() {
	rm -f .tmp_file
    }

    # We use this fixed file-name...
    # I hardly think anybody would like to
    # seriously create a file named this
    sed -e "/$_LINE_END/Q" $f > .tmp_file

    {
    case $ftype in
	fortran)
	    echo "! project-$_LINE_END"
	    echo "!     Anything below this line may be overwritten by scripts"
	    echo "!     Below are non-editable settings"
	    ;;
    esac
    # Empty line
    echo ""
    case $ftype in
	fortran)
	    echo "! Local Variables:"
	    # Note that if users override the f90
	    # format name, we cannot do anything about it...
	    echo "!  mode: f90"
	    for key in ${!fv[@]}
	    do
		echo "!  $key: ${fv[$key]}"
	    done
	    echo "! End:"
	    ;;
	*)
	    echo "Error, could not figure out file type"
	    clean_tmp
	    exit 0
	    ;;
    esac
    echo ""
    } >> .tmp_file
    mv .tmp_file $f
}


for d in $@
do
    while read f
    do
	f=${f// /}
	[ -z "${f}" ] && continue
	update_setting $d/$f
    done < $d/files.fortran
done
