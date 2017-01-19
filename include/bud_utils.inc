  ! @@LICENSE@@ see Copyright notice in the top-directory

! Utilities for pre-processing source-files
! in the bud library.

! Small check whether it already has been included.
#ifndef BUD_UTILS_H
#define BUD_UTILS_H

  ! function name for new routines
  ! Although all interfaces need not have explicit names
  ! we allow both the "new" and the "new<T>" construct
  ! for clarity.

#define BUD_NEW new

#define BUD_MOD bud

#define BUD_TYPE

#ifndef BUD_FORTRAN
# define BUD_FORTRAN 1995
#endif

#ifndef BUD_FORTRAN_TARGET
# define BUD_FORTRAN_TARGET , target
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

# ifndef BUD_FORTRAN_CONTIGUOUS
#  define BUD_FORTRAN_CONTIGUOUS
# endif

# define BUD_CLASS type

#endif


#define BUD_CC2(a,b) BUD_CCX2(a,b)
#define BUD_CCX2(a,b) a ## b

#define BUD_CC3(a,b,c) BUD_CCX3(a,b,c)
#define BUD_CCX3(a,b,c) a ## b ## c

#define BUD_CC4(a,b,c,d) BUD_CCX4(a,b,c,d)
#define BUD_CCX4(a,b,c,d) a ## b ## c ## d

#define BUD_CC5(a,b,c,d,e) BUD_CCX5(a,b,c,d,e)
#define BUD_CCX5(a,b,c,d,e) a ## b ## c ## d ## e

#define BUD_CC6(a,b,c,d,e,f) BUD_CCX6(a,b,c,d,e,f)
#define BUD_CCX6(a,b,c,d,e,f) a ## b ## c ## d ## e ## f

#define BUD_XSTR(x) BUD_STR(x)
#define BUD_STR(x) #x

#define BUD_DIM_SPEC1(x) (x)
#define BUD_DIM_SPEC2(x) (x,x)
#define BUD_DIM_SPEC3(x) (x,x,x)
#define BUD_DIM_SPEC4(x) (x,x,x,x)
#define BUD_DIM_SPEC5(x) (x,x,x,x,x)
#define BUD_DIM_SPEC6(x) (x,x,x,x,x,x)

#endif

  ! Check that the DIM_SPEC has not been specified
#undef BUD_DIM_SPEC
#if defined(BUD_DIM)
# if BUD_DIM == 0
#  define BUD_DIM_SPEC(a)
# elif BUD_DIM == 1
#  define BUD_DIM_SPEC(a) (a)
# elif BUD_DIM == 2
#  define BUD_DIM_SPEC(a) (a,a)
# elif BUD_DIM == 3
#  define BUD_DIM_SPEC(a) (a,a,a)
# elif BUD_DIM == 4
#  define BUD_DIM_SPEC(a) (a,a,a,a)
# elif BUD_DIM == 5
#  define BUD_DIM_SPEC(a) (a,a,a,a,a)
# elif BUD_DIM == 6
#  define BUD_DIM_SPEC(a) (a,a,a,a,a,a)
# endif
#else
# define BUD_DIM_SPEC(a)
#endif


  ! project-buds -- local file settings
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

