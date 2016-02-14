#!/bin/bash

# Simple function for checking the final preprocessed
# code.

# Variable containing the pre-processor command
_PP=

function set_pp_cmd {
    _PP="$@"
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
    $_PP $file > pp_$file
}

while [ $# -gt 0 ]
do
    pre_process $1
    shift
done
