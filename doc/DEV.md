# Development  {#devel}

- @ref develNotes
- @ref develNew


A short introduction to the buds library and how it may be
extended can be found in this section.

We highly welcome suggestions, issues and/or bug-fixes.
Anybody is encouraged to contribute via the github development page:

- [Development repository][buds]
- [Reporting issues/bugs][issues]
- [Creating pull-requests for buds][pr]

Please do not hesitate to contribute!

# Notes for developers  {#develNotes}

The library is heavily pasted with pre-processor statements
which, sadly, is not well-supported in a large range of
fortran compilers.  
As such we require a minimal usage of external preprocessors
which adhere to the C preprocessor rules.  
Any development should adhere to the standards found in the
already present buds.  
Additionally these guidelines __must__ be followed:

- Indent in a 2-level sequence.
  + Currently this is enabled for emacs via local variables
  + Please copy-paste the bottom comments into any new files
    to follow these rules.

- Pre-processor variables __must__ adhere to these rules:
  1. Prefix with `BUD_` which ensures no name-clashing with
  parenting libraries/applications.
  2. Ensure the implementation files contain an `#undef` statement
  to limit warnings when compiled.
  Although this is not necessary it eases debugging
  substantially.

- All public procedures (subroutine/function) __must__ be interfaced.
  I.e. no direct procedure may be made public.

- Use the Doxygen documentation format for every procedure.

  If you do not know how Doxygen works, please look in the documentation
  for a routine/function which you think replicates your routine the most
  and copy/paste the corresponding documentation.

  Importantly, the main documentation should be present at the interface
  construct and _not_ at the internal deferred routine. 

  Generally these guidelines are encouraged:
  + Document parameters (`@param`/`@return`) at private procedures
  + Document interface procedures with _actual_ documentation of
    procedure intent and usage.
    In case several procedures are interfaced this may be added to the
	interface documentation.
  + Create references for F2003 OO procedures to the equivalent interface.
~~~~~~~~~~~{.f90}
          ...
       #if BUD_FORTRAN >= 2003
        contains
		  !> @iSee `new`
          procedure, public :: new => new_
	   #endif
	    end type

        !> Here goes full documentation
		interface new
          module procedure new_
		end interface
~~~~~~~~~~~
    You may also define later pre-processor statements using later revisions.

  + Add contribution statements in the `CONTRIBUTORS.md` file
	which ensures a resulting documentation to be short and concise.  
    In the `CONTRIBUTORS.md` file you may add any information and specifics
	of what has been implemented by specific authors.
  + Do __not__ use the `@date` marker as it clutters the documentation with
    limited use.
  + Use `\@opt` to denote optional arguments, for instance:
~~~~~~~~~~~{.f90}
		!! @param[inout] bar1 updated value
		!! @param[in] bar2 \@opt=\@null decide how bar1 calculated.
		subroutine foo_(bar1, bar2)
		  ..., intent(inout) :: bar1
		  ..., intent(in), optional :: bar2 ! none default
~~~~~~~~~~~

  + Use `\@null` as replacement for optional arguments which may have no information
  + To remove routines or hidden variables from the documentation you may
    encompass part of the code with
~~~~~~~~{.f90}
	  !> \@cond ALWAYS_SKIP
       ... ! skipped documentation in doxygen
      !> \@endcond ALWAYS_SKIP
~~~~~~~~
    This does not prohibit the requirement of adding documentation as it is
	still available for developers.

- _Only_ use functions for single value retrievals. Generally subroutines
  have less overhead and are preferred over functions.  
  However, functions have their usage as they have a clear
  intent.

- Procedures (functions/subroutines) should be prefixed with:
  + `get_?` for retrieval of values/data in the bud.
  + `ptr_?` for retrieval of a pointer to the data in the bud.
  This forces the routines to be made into
  to interfaced procedures.  
  Furthermore it enables the use of the actual procedure
  name as local variables.



## Understanding buds implementation  {#develUnderstand}

All _buds_ are heavily relying on the `.inc` files in
the `include/` directory.

The dependency graph for the intrinsic _bud_ includes can be seen here:

![Dependency graph for include/bud_*.inc](buds_include.png)

The `bud_utils.inc` file is globally dependent. It exposes
several generic macros for concatenating, default names, etc.  
When creating a new _bud_, including this utility function
will most likely be a requirement.  

Each arrow shows _usage_ direction. I.e. `bud_coll.inc` has at least two `#include` statements.

1. One or more of the `common*.inc` are included
2. `#include bud_coll_elem.inc`


### CPP, a problematic code generation tool? {#develCPP}

The decision on using pre-processors to construct the
_buds_ library was not a dream situation. Ideally the
creation of new buds should be easily understood, and also
easy to do.  
This has been difficult to achieve as the number of
intrinsic control definitions (`#define`) keeps increasing.
The CPP method involves a linear creation of the code which
makes pre-processors non-ideal due to their limitation in
branching constructs. Especially the lack of string comparison
makes it difficult to use.

I did initially think about creating a bash-equivalent
meta-scripting language for dynamic code generation.
However, this will greatly limit the creation of new _buds_
as the API for such construction will be foreign for a
majority of developers.

Hence the final decision went to the pre-processors.

There are many other choices considering code generation tools:

- m4
- Python
- Bison/YACC
- etc.


# Creating a new bud  {#develNew}

A basic bud is fully implemented by the developer and
should only rely on the `common*.inc` files as seen in
[develUnderstand].

Here is a walk-through of creating a bud with a single
variable contained.
Note that the following points also represents the
structure of the code file. I.e. you should retain the order
of points in the code.

1. The first code present in the type _must_ be
~~~~~~{.c}
    #include "bud_utils.inc"
~~~~~~

2. Define your _bud_ name (these names _should_ be used):
~~~~~~{.c}
    #define BUD_TYPE_NAME OneVariable
    #define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
~~~~~~
   The `BUD_CC2` macro is enabled in `bud_utils.inc`.	

3. Add common declarations:
~~~~~~~~{.c}
    #include "bud_common_declarations.inc"
~~~~~~~~
   This adds the common used variables such as
   data precisions, default bud variables etc.
	
4. Define the data container (`BUD_TYPE_NAME_`)

	This data type may contain anything you want but
	__must__ be defined as this:
~~~~~~~{.f90}
	type BUD_TYPE_NAME_  ! Doxygen lower-cases this :(
      <contained data>
    #include "buds_common_type_.inc"
    end type
~~~~~~~
   In this case `<contained data>` can be anything
   you desire. An intrinsic fortran type, a derived type,
   another bud, etc.

5. Define the public derived type (`BUD_TYPE_NAME`)

	This data should _only_ contain this:
~~~~~~~{.f90}
	type BUD_TYPE_NAME  ! Doxygen lower-cases this :(
      type(BUD_TYPE_NAME_), pointer :: D => null()
    #if BUD_FORTRAN >= 2003
    contains
	#   include "bud_common_type.inc"
      <procedures exposed via Object-Oriented programming>
    #endif
    end type
~~~~~~~
   Generally we advocate adding the OO-procedures.

6. Here the interface definitions are added.  
	Also add the documentation of the interfaces.  
   Explicitly add `public` or `private` for clarity of the
   interfaced routines.  
   Sometimes it may be handy to create interfaces that
   are private.

7. Add the common buds interfaces and routines.
~~~~~~~{.c}
   #include "buds_common.inc"
~~~~~~~

   Remark that this inclusions inserts a `contains`
   statement which forces the separation of variable/interface declarations
   from routine declarations.
   Hence all interfaces and constants _must_ be defined before this point,
   and all code must be present after this point.

8. Create a deletion routine which shall ensure no
   memory leaks. I.e. it should delete _all_ allocatable/pointers in the
   `BUD_TYPE_NAME_` type.  
   Its interface __must__ look like this:
~~~~~~~{.f90}
   subroutine delete_data(D)
     type(BUD_TYPE_NAME_), intent(inout) :: D  ! Doxygen lower-cases this :(
     <clean-up>
   end subroutine
~~~~~~~

9. Add any specific routines for your data type. Note that you may
   never expose routines directly from the module. This will ensure
   that name-clashes are never encountered.



[buds]: https://github.com/siesta-project/buds
[issues]: https://github.com/siesta-project/buds/issues
[pr]: https://github.com/siesta-project/buds/pulls
