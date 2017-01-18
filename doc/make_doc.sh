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
DOC=${DOC:doxygen}

# Retrieve the git-source top-directory
_main_dir=`git rev-parse --show-toplevel`

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
    have_dot="HAVE_DOT = YES"

    # Make sure we create all custom graphs for
    # dependency graphs
    dot -Tpng include/buds_include.dot > doc/images/buds_include.png
    
else
    have_dot="HAVE_DOT = NO"
fi


# First we need to ensure the sources are ok.
mkdir $_SRC
pushd $_SRC
{
    echo "TOP_DIR = $_main_dir"
    # Enable MPI routines for documentation
    echo "MPI = 1"
    # Enable OO procedures for documentation
    echo "OO = 1"
    echo "include \$(TOP_DIR)/Makefile"
} > Makefile
make source
# Ensure the *.inc files are also present
cp $_main_dir/include/*.inc .
popd

# Now we can create the documentation
# This will create the documentation folder
case $DOC in
    
    doxygen|Doxygen)
	{
	    cat doc/Doxyfile
	    # Insert correct documentation version
	    echo "PROJECT_NUMBER = $doc_version"
	    echo "$have_dot"
	} | doxygen -

	# Insert the version string in the version
	sed -i -e "s/BUDS_VERSION/$doc_version/g" html/index.html
	# This reflects the DOWNLOAD.md file
	sed -i -e "s/BUDS_VERSION/$tar_version/g" html/download.html

	;;
    
    ford|FORD)
	ford doc/ford.md
	;;
    
esac

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
