# Development of buds

A short introduction to the buds library and how it may be
extended can be found in this section.

We highly welcome suggestions, issues and/or bug-fixes.
Anybody is encouraged to contribute via the github development page:

- [General development repository][buds]
- [Reporting issues/bugs][issues]
- [Creating pull-requests for buds][pr]

Please do not hesitate to contribute!

## Notes for developers

The library is heavily pasted with pre-processor statements
which, sadly, is not well-supported in a large range of
fortran compilers.  
As such we require a minimal usage of external preprocessors
which adhere to the C preprocessor statements.  
Any development should adhere to the standards found in the
already present buds.  
These guide-lines for the code __must__ be followed:

- Indent in a 2-level sequence.
  + Currently this is enabled for emacs via local variables

- Pre-processor variables __must__ adhere to these rules:
  1. Prefix with `BUD_` which ensures no name-clashing with
     parenting libraries/applications.

- All public routines/functions __must__ be interfaced. I.e.
  no routines/functions may be made public.

- Documentation using the Doxygen format for every routine.
  If you do not know how Doxygen works, please look in the documentation
  for a routine/function which you think replicates your routine the most
  and copy/paste the corresponding documentation.

  + Prefer to add contribution statements in the CONTRIBUTORS.md file
	which ensures a resulting documentation to be short and concise.  
    In the CONTRIBUTORS.md file you may add any information and specifics
	of what has been implemented by specific authors.
  + Do __not__ use the `\@date` marker as it clutters the documentation.
  + Use `\@opt` to denote optional arguments, for instance:
~~~~~~~~~~~{.f90}
		!! @param[inout] bar1 updated value
		!! @param[in] bar2 \@opt=\@null decide how bar1 calculated.
		subroutine foo(bar1, bar2)
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

- *Only* use functions for single value retrievals. Generally subroutines
  have less overhead and are preferred over functions.  
  However, functions have their usage as they have a clear
  intente.

- Functions should be named `get\_?` to enable the 
  function name as a local variable.  
  It also helps understanding the code *in-line*.
  
## Creating a new bud

A short description of the required steps needed to
create a custom bud type is listed here:

1. The first code present in the type _must_ be
~~~~~~{.f90}
    #include "bud_utils.inc"
~~~~~~

2. Define your internal data container. Its name **must** be
   `BUD_TYPE_NAME_`:
~~~~~~{.f90}
	#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
~~~~~~
   This data container name will only be visible
   in the module and thus it is relatively un-important
   what you choose.

   We, however, recommend that you re-use the public
   type name with an appended `_`.
	
3. Add the common declarations:
~~~~~~~~{.f90}
    #include "bud_common_declarations.inc"
~~~~~~~~
   This adds the common used variables such as
   data precisions etc.
	
4. Define the data container (`BUD_TYPE_NAME_`)

	This data type may contain anything you want but
	**must** be defined as this:
~~~~~~~{.f90}
	type BUD_TYPE_NAME_
     <contained data>
    #include "buds_common_type.inc"
    end type
~~~~~~~

5. Add specific interfaces that you may implement for
   interacting with the contained data.  
   Ensure that you add a one-line documentation to the interfaces.
   Explicitly add `public` or `private` for clarity of the
   interfaced routines.

6. Add the common buds interfaces and routines.
~~~~~~~{.f90}
   #include "buds_common.inc"
~~~~~~~

   Remark that this inclusions inserts a `contains`
   statement which forces the separation of variable/interface declarations
   from routine declarations.

7. Create a deletion routine which shall ensure no
   memory leaks. I.e. it should delete *all* allocatable/pointers in the
   `BUD_TYPE_NAME_` type.  
   This routine **must** be named `delete_data`.

8. Add any specific routines for your data type. Note that you may
   never expose routines directly from the module. This will ensure
   that name-clashes are never encountered.


## Acknowledgements

The basic concept of the reference counted mechanisms is inspired by
the FLIBS project by Arjen Markus as well as minor things from the PyF95 project.

The initial draft of this library was created by Alberto Garcia.


[buds]: https://github.com/siesta-project/buds
[issues]: https://github.com/siesta-project/buds/issues
[pr]: https://github.com/siesta-project/buds/pulls

