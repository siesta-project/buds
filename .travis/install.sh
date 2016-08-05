#!/bin/bash

# Run installation
echo "CPP version"
cpp --version
echo "gfortran version"
gfortran --version
# Show help from Makefile
make show
# Actually build fdict
if [ -z "$BUDS_OPT" ]; then
   OPT=0
else
   OPT="$BUDS_OPT"
fi
make OPT=$OPT
