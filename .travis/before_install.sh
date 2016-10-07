#!/bin/bash

# Do pre-install commands
echo "OS-name: $TRAVIS_OS_NAME"

function buds_osx {
    brew update
    brew install gcc
    # If gfortran install fails do nothing
    brew install gfortran || echo "Should be OK"
    {
      echo CPP = gcc -E -P -x c
    } > setup.make
}

function buds_linux {
    sudo apt-get install -qq gfortran
}

case "$TRAVIS_OS_NAME" in
   osx)
    buds_osx
    ;;
   *)
    buds_linux
    ;;
esac

