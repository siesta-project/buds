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

# Check whether we should use dot
if [[ $(have_exe dot) -eq 0 ]]; then
    have_dot="HAVE_DOT = YES"
else
    have_dot="HAVE_DOT = NO"
fi

# Start on the actual creation of the documentation
# We create a temporary directory
rm -rf $_DOC

# Now we can create the documentation
{
    cat doc/Doxyfile
    echo "$have_dot"
} | doxygen -



###################
# Post-processing #
###################
pushd $_DOC

# if we have optipng we optimize the PNG's
# to reduce webpage size.
if [[ $(have_exe optipng) -eq 0 ]]; then
    find ./ -name "*.png" | \
	xargs -n 1 -P 4 optipng -o7
fi

popd