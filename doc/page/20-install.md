title: Install

# Installation instructions  {#installInstructions}

- [Basic installation](#installBasic)
- [Advanced installation](#installAdvanced)
- [Compiler settings](#installSettings)
- [Functionality](#installFunctionality)
- [Compiler vendors](#installVendor)


# Basic Installation  {#installBasic}

Building buds is only controlled using the basic Makefiles included.
Every setting for the compilation and installation can be controlled
using the `setup.make` file.

If you are using the standard GNU compiler suite you may compile
and install buds using the following commands:

    make
    make install PREFIX=<install prefix>

the default `PREFIX` is `$HOME/buds`.

This basic installation will only install the static library.
To control whether the shared/static or both libraries should be installed
one can do

    make all

To _only_ compile the shared library one should do:

    make shared


The installation will install either library and the basic buds include
files in the PREFIX directory.


## Advanced installation  {#installAdvanced}

The actual control of the installation script can be fully controlled in the
`setup.make` file.
An assortment of pre-make files are created in the setup.makes directory.

To use one of them you may link it to `setup.make`:

    ln -s setup.makes/static-hard.make setup.make
    make

which will use the settings for the static library using an aggressive
optimization strategy.

You may also denote the `setup.make` file using the `SETUP` flag:

    make SETUP=static-hard # or
    make SETUP=static-hard.make # or
    make SETUP=setup.makes/static-hard.make

To check your current installation setup run this command:

    make show


## Compiler settings  {#installSettings}

Either of these settings may be provided on the `make` command line
or in `setup.make`.
In the following 0 means disable while 1 means enable.

* PREFIX=<installation prefix>:

  installation prefix for buds.

* STATIC=1|0:  

  control whether the `libs` target is dependent on the `static` target
    
* SHARED=0|1:

  control whether the `libs` target is dependent on the `shared` target

* OPT=|debug|1|2|3:

  controls the optimization level of the compiler.
  Debug is the lowest compiler options, 1 is low optimization and
  3 is the highest optimization.

* VENDOR=gnu|intel|<vendor>:

  use the settings in the vendor/<vendor>.make file for the
  compiler (see [vendors](#installVendor)).

* SETUP=<file>:

  use the specified file in addition to the `setup.make` file
  to control the options. The corresponding file will be included in the
  makefile process right _after_ `setup.make`.

* MPI=0|1:

  control whether the MPI objects should also be added to the library.
  This enables certain buds only available for MPI installations.

* OO=1|0:

  control whether the objects are OO so that there exists both regular
  procedures and the `<>%%<procedure>` calls.
  This enables object oriented programming.


# Library functionality  {#installFunctionality}

There are several preprocessor flags for controlling the final
buds library. Some of them extends the functionality while others
limit the functionality.
Currently the preprocessor flags must be added to the CPPFLAGS variable:

    CPPFLAGS = -D<preprocessor-flag>

Add these to the CPPFLAGS controlling

* `-DBUD_FORTRAN=<integer>`:

    This controls the fortran standard used.
    Currently if <integer> is 2003 or above the basic buds
    will contain OO functionality as `bud%%is_initd()` together
    with the direct function call `is_initd(bud)`.

    This is defaulted to 2003 as the majority of compiler vendors
    intrinsically support these extensions.

* `-DBUD_ERROR=<routine name>`:

    Controls whether the parent application will provide a
    routine for grabbing errors.
    This routine should be an external routine (not in a module).
    The interface for this error *MUST* be

        subroutine BUD_ERROR(str)
         character(len=*), intent(in), optional :: str
        end subroutine



# Compiler vendors  {#installVendor}

The `vendor` directory contains individual files that creates
defaults for different vendors.  
If you want to enable a new compiler you may copy the `vendor/gnu.make`
to `vendor/<new>.make` and customize the flags to your liking.

Once you have created the vendor file you may use the vendor by compiling
using:

    make VENDOR=<new>

Currently only GNU and Intel compiler vendors are supported:

    make VENDOR=gnu # default
    make VENDOR=intel

If you add a new vendor please consider contributing to buds by adding
a [pull request][buds-pr].





<!--
You need not read below these lines, they are used for the
documentation
-->

[buds-pr]: https://github.com/siesta-project/buds/pulls

