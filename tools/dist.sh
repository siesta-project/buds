#!/bin/bash

#
# Distribution creation tool
#
_tmpdir=.tmpd
if [ -d $_tmpdir ]; then
    echo "Please remove $_tmpdir from the top directory..."
    exit 1
fi


# This *MUST* be called from the top-directory.

# Get tag branch and number of commits since
describe=`git describe HEAD 2>/dev/null`
if [ $? -ne 0 ]; then
    describe=`git rev-parse --short --verify HEAD`
else
    # Remove hash
    describe=${describe%-*}
fi

# Create file that we are going to store the distribution
# in
file=buds-$describe.tar.gz
# Clean it
rm -f $file

# Create the archive (with prefix)
git archive --prefix buds-$describe/ \
    --format tar.gz \
    -o $file HEAD

# Add the pre-processed files in case the user
# does not have a sufficient pre-processor.
mkdir $_tmpdir
pushd $_tmpdir
tar xfz ../$file
cp ../src/*.f90 */src/
tar cfz $file ${file//.tar.gz/}
mv $file ../
popd
rm -rf $_tmpdir

