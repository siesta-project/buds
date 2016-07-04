  ! @@LICENSE@@ see Copyright notice in the top-directory

! Utilities for pre-processing source-files
! in the bud library.

! Small check whether it already has been included.
#ifndef BUD_UTILS_H
#define BUD_UTILS_H

  !> @cond BUD_DOC_INC

  !> \defgroup buds-utils Preprocessor utility functions
  !!
  !! Documentation of utility functions that may be
  !! used by adding this to a @bud
  !! \code{.f90}
  !! # include "bud_utils.inc"
  !! \endcode
  !!
  !! These preprocessor macros may be used to easily
  !! concatenate or stringify macros or constants.
  !!
  !! This include file also contains
  !! default preprocessor definitions of the
  !! prefix for modules etc.
  !!
  !! @{
  
  ! function name for new routines
  ! Although all interfaces need not have explicit names
  ! we allow both the "new" and the "new<T>" construct
  ! for clarity.

  !> \def BUD_NEW
  !! The name for creating a new @bud
#define BUD_NEW new
  
  !> \def BUD_MOD
  !! The prefix used for @bud modules.
  !!
  !! Defaulted to `new`
#define BUD_MOD bud

  !> \def BUD_TYPE
  !! The prefix used for @bud types, this should generally not be altered.
  !!
  !! Default to `` (empty).
#define BUD_TYPE 

  !> \def BUD_FORTRAN
  !! Denote the fortran standard that the compiler supports.
  !!
  !! This will default to 2003 and will directly enable the
  !! use of OO programming paradigms for the @bud.
  !!
  !! Essentially every @bud allows calls such as:
  !! \code{.f90}
  !! type(Bud) :: bu
  !! call new(bu)
  !! ! or
  !! call bu%new()
  !! \endcode
  !! where the developer creates interfaces for both.

#ifndef BUD_FORTRAN
# define BUD_FORTRAN 2003
#endif

#if BUD_FORTRAN >= 2003
  ! In case BUD_FORTRAN version is above 2003
  ! we have a couple of different specifications
  ! that are standard and common.
  
# ifndef BUD_FORTRAN_FINALIZE
#  define BUD_FORTRAN_FINALIZE 1
# endif

# ifndef BUD_FORTRAN_CONTIGUOUS
#  define BUD_FORTRAN_CONTIGUOUS , contiguous
# endif

# define BUD_CLASS class

#else

# define BUD_FORTRAN_CONTIGUOUS
# define BUD_CLASS type
  
#endif


  !> \def BUD_CC2(a,b)
  !! Concatenate two arguments \a a and \a b.
#define BUD_CC2(a,b) BUD_CCX2(a,b)
#define BUD_CCX2(a,b) a ## b
  
  !> \def BUD_CC3(a,b,c)
  !! Concatenate three arguments \a a, \a b and \a c.
#define BUD_CC3(a,b,c) BUD_CCX3(a,b,c)
#define BUD_CCX3(a,b,c) a ## b ## c
  
  !> \def BUD_CC4(a,b,c,d)
  !! Concatenate four arguments \a a, \a b, \a c and \a d.
#define BUD_CC4(a,b,c,d) BUD_CCX4(a,b,c,d)
#define BUD_CCX4(a,b,c,d) a ## b ## c ## d
  
  !> \def BUD_CC5(a,b,c,d,e)
  !! Concatenate five arguments \a a, \a b, \a c, \a d and \a e.
#define BUD_CC5(a,b,c,d,e) BUD_CCX5(a,b,c,d,e)
#define BUD_CCX5(a,b,c,d,e) a ## b ## c ## d ## e

  !> \def BUD_XSTR(x)
  !! Creates a string of the expanded argument.
#define BUD_XSTR(x) BUD_STR(x)
  
  !> \def BUD_STR(x)
  !! Creates a string of the argument.
#define BUD_STR(x) #x

  !> @}

  !> @endcond BUD_DOC_INC
  
#endif

  ! Check that the DIM_SPEC has not been specified
#if defined(BUD_DIM) && ! defined(BUD_DIM_SPEC)
# if BUD_DIM == 0
#  define BUD_DIM_SPEC()
# elif BUD_DIM == 1
#  define BUD_DIM_SPEC() (:)
# elif BUD_DIM == 2
#  define BUD_DIM_SPEC() (:,:)
# elif BUD_DIM == 3
#  define BUD_DIM_SPEC() (:,:,:)
# elif BUD_DIM == 4
#  define BUD_DIM_SPEC() (:,:,:,:)
# elif BUD_DIM == 5
#  define BUD_DIM_SPEC() (:,:,:,:,:)
# endif
#endif

  
  ! project-bud -- local file settings
  !     Anything below this line may be overwritten by scripts
  !     Below are non-editable settings

  ! Local Variables:
  !  mode: f90
  !  f90-if-indent: 2
  !  f90-type-indent: 2
  !  f90-associate-indent: 2
  !  f90-continuation-indent: 2
  !  f90-structure-indent: 2
  !  f90-critical-indent: 2
  !  f90-program-indent: 2
  !  f90-do-indent: 2
  ! End:

