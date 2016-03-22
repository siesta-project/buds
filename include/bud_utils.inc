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
#define BUD_MOD mbud
!#define BUD_TYPE tbud_
#define BUD_TYPE 

  ! Define the used fortran standard
#ifndef BUD_FORTRAN
# define BUD_FORTRAN 2003
#endif

#ifndef BUD_FORTRAN_FINALIZE
# if BUD_FORTRAN >= 2003
#  define BUD_FORTRAN_FINALIZE 1
# endif
#endif

  ! If we are using F03 we may use the class statements
#if BUD_FORTRAN >= 2003
# define BUD_CLASS class
#else
# define BUD_CLASS type
#endif


! Concatenation
#define BUD_CC2(a,b) BUD_CCX2(a,b)
#define BUD_CCX2(a,b) a ## b
#define BUD_CC3(a,b,c) BUD_CCX3(a,b,c)
#define BUD_CCX3(a,b,c) a ## b ## c
#define BUD_CC4(a,b,c,d) BUD_CCX4(a,b,c,d)
#define BUD_CCX4(a,b,c,d) a ## b ## c ## d
#define BUD_CC5(a,b,c,d,e) BUD_CCX5(a,b,c,d,e)
#define BUD_CCX5(a,b,c,d,e) a ## b ## c ## d ## e


! To stringify
#define BUD_XSTR(x) BUD_STR(x)
#define BUD_STR(x) #x


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

