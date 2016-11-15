#!/bin/bash


# This script enables certain cleaning for the
# sources in the buds library.

# Figure out the top directory of the git repo
top_dir=`git rev-parse --show-toplevel`

# This function
# takes a single source code as argument
# and will _in-place_ remove all trailing
# white-space characters on ALL lines.
function trailing_white {
    local file=$1
    shift
    echo "Removing trailing white-space from: $file"
    sed -i -e 's:[[:space:]]*$::g' $file
}


function print_help {
    echo "$0 can be called with these options:"
    echo ""
    echo "  trailing-white:"
    echo "    removes any trailing white-space in ALL sources"
}

TRAILING_WHITE=1
method=0

case $1 in
    trailing-white)
	method=$TRAILING_WHITE
	;;
    help)
	print_help
	exit 0
	;;
    *)
	print_help
	exit 1
	;;
esac


if [ $method -eq $TRAILING_WHITE ]; then
    pushd $top_dir

    for f in **/*.F90 **/*.f90 **/*.inc **/*.F **/*.f
    do
	# In case there are no files with the
	# file-ending
	[ ! -e $f ] && continue
	trailing_white $f
    done
    popd
fi
	


