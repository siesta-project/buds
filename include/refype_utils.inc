  ! @@LICENSE@@ see Copyright notice in the top-directory

! Utilities for pre-processing source-files
! in the refype library.

! Small check whether it already has been included.
#ifndef _R_UTILS_H
#define _R_UTILS_H


! function name for new routines
! Although all interfaces need not have explicit names
! we allow both the "new" and the "new<T>" construct
! for clarity.
#define _R_NEW new
#define _R_MOD mryp
!#define _R_TYPE tryp_
#define _R_TYPE 


! Concatenation
#define _R_CC2(a,b) _R_CCX2(a,b)
#define _R_CCX2(a,b) a ## b
#define _R_CC3(a,b,c) _R_CCX3(a,b,c)
#define _R_CCX3(a,b,c) a ## b ## c
#define _R_CC4(a,b,c,d) _R_CCX4(a,b,c,d)
#define _R_CCX4(a,b,c,d) a ## b ## c ## d
#define _R_CC5(a,b,c,d,e) _R_CCX5(a,b,c,d,e)
#define _R_CCX5(a,b,c,d,e) a ## b ## c ## d ## e


! To stringify
#define _R_XSTR(x) _R_STR(x)
#define _R_STR(x) #x


#endif


  ! project-refype -- local file settings
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

