#!/bin/bash

# This small script creates the documentation
# in the same directory.

# It *MUST* be runned from the top directory of the
# sources.
# It will warn if this is not the case

_DOC=final_doc

# NOTE if any user may end up symlinking the script, this
# will not work. However, who would do that?
_sd="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

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


if [[ $(dirname $_sd) != $_cwd ]]; then
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
    printf "%d" $?
}
    

# Assert that the used executables are available:
_fail=""
for exe in doxygen
do
    if [[ $(have_exe $exe) -ne 0 ]]; then
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
if [[ $(have_exe dot) -eq 0 ]]; then
    have_dot="HAVE_DOT = YES"

    # Make sure we create all custom graphs for
    # dependency graphs
    dot -Tpng include/buds_include.dot > doc/images/buds_include.png
    
else
    have_dot="HAVE_DOT = NO"
fi

# Start on the actual creation of the documentation
# We create a temporary directory
rm -rf $_DOC

# Now we can create the documentation
{
    cat doc/Doxyfile
    # Insert correct documentation version
    echo "PROJECT_NUMBER = $doc_version"
    echo "$have_dot"
} | doxygen -




###################
# Post-processing #
###################
pushd $_DOC

# Insert the version string in the version
sed -i -e "s/BUDS_VERSION/$doc_version/g" html/index.html
# This reflects the DOWNLOAD.md file
sed -i -e "s/BUDS_VERSION/$tar_version/g" html/download.html

# if we have optipng we optimize the PNG's
# to reduce webpage size.
if [[ $(have_exe optipng) -eq 0 ]]; then
    find ./ -name "*.png" | \
	xargs -n 1 -P 4 optipng -o7
fi

popd
