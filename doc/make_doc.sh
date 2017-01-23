#!/bin/bash

# This small script creates the documentation
# in the same directory.

# It *MUST* be runned from the top directory of the
# sources.
# It will warn if this is not the case

# Default parameters determining which sub-programs
# should be used (mainly for developing documentation)
#   whether graphviz should be used...
DOT=${DOT:-1}
#   whether graphviz should be used...
OPTIPNG=${OPTIPNG:-1}

# Retrieve the git-source top-directory
_main_dir=`git rev-parse --show-toplevel`


# Specify the documentation and source directories
_DOC=$_main_dir/buds-final-doc
_SRC=$_main_dir/buds-final-src

# Initially clean the documentation and source
# directories
rm -rf $_DOC $_SRC


pushd $_main_dir

# Get the calling directory
_cwd=`pwd`

# Get latest tag version on master
tag=`git describe --abbrev=1`
doc_version=${tag%-*}
# Retrieve end
end=${doc_version#*-}
tar_version=${doc_version%%-*}
if [[ -z "$end" ]]; then
    doc_version=$tar_version
else
    doc_version="$tar_version (+$end)"
fi
unset end


if [[ $_main_dir != $_cwd ]]; then
    echo "$0 must be called from this directory:"
    echo "  $(dirname $_sd)"
    echo "Currently it is called from:"
    echo "  $_cwd"
    exit 1
fi

function have_exe {
    local exe=$1
    shift
    which $exe > /dev/null
    return $?
}
    

# Assert that the used executables are available:
_fail=""
for exe in doxygen
do
    if ! $(have_exe $exe) ; then
	_fail="$_fail $exe"
    fi
done
if [[ -n "$_fail" ]]; then
    echo "The following executables could not be found:"
    echo ""
    echo "  $_fail"
    echo ""
    echo "They are required for creating the documentation."
    exit 1
fi

mkdir -p doc/images

# Check whether we should use dot
if $(have_exe dot) && [ $DOT -eq 1 ] ; then
    use_graph="true"

    # Make sure we create all custom graphs for
    # dependency graphs
    dot -Tpng include/buds_include.dot > doc/images/buds_include.png
    
else
    use_graph="false"
fi


# First we need to ensure the sources are ok.
mkdir -p $_SRC
pushd $_SRC
{
    echo "TOP_DIR = $_main_dir"
    # Enable MPI routines for documentation
    echo "MPI = 1"
    # Enable OO procedures for documentation
    echo "OO = 1"
    echo "include \$(TOP_DIR)/Makefile"
} > Makefile

make -j4 source

# Ensure the *.inc files are also present
cp $_main_dir/include/*.inc .

# Clean-up source comments (not documentation)
sed -s -i -e '/^[ ]*! /d' *.f90 *.inc

#rm bud_[^M]*

popd

# Now we can create the documentation
# This will create the documentation folder
sed -e "s/%%VERSION%%/$doc_version/g;
s/%%GRAPH%%/$use_graph/g" doc/FORD.md > doc/FORD_V.md
ford --debug doc/FORD_V.md && rm doc/FORD_V.md

# Insert the version string in the documentation
sed -i -e "s/BUDS_VERSION/$tar_version/g" $_DOC/page/10-download.html


###################
# Post-processing #
###################
pushd $_DOC

# if we have optipng we optimize the PNG's
# to reduce webpage size.
if $(have_exe optipng) && [ $OPTIPNG -eq 1 ] ; then
    find ./ -name "*.png" | \
	xargs -n 1 -P 4 optipng -o7
fi

popd

popd
