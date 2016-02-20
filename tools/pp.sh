#!/bin/bash

# Simple function for checking the final preprocessed
# code.

# Variable containing the pre-processor command
_PP=

# File-name prefix
PREFIX=${PREFIX:-pp_}
# Remove white-space
PREFIX=${PREFIX// /}

function set_pp_cmd {
    _PP="$@ -I../include"
    
}

# Default to this command:
#  -E only preprocess, do not try and compile
#  -P do not add line markers
#  -C is needed to disregard C-comments (the operator // is a comment!?!)
#  -nostdinc is needed to not include standard includes (which are C comments)
set_pp_cmd cpp -E -P -C -nostdinc

function pre_process {
    local file=$1
    shift
    # re-create file with correct ending
    local b=$(basename $file)
    $_PP $file > $PREFIX$b
}

while [ $# -gt 0 ]
do
    pre_process $1
    shift
done
