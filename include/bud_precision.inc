  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Just for the concatenation features
#include "bud_utils.inc"
  
#ifndef BUD_PRECISION_SUFFIX
# define BUD_PRECISION_SUFFIX 
#endif

  ! Data precisions (PRIVATE)
  ! Although these names may be "uncommon"
  ! they are only internal variables and should only be used as such.
  ! They represent the 3 allowed integer variables:
  !  is == short (2-byte)
  !  ii == int (4-byte)
  !  il == long (8-byte)
  ! and the real variables
  !  rr == real (4-byte)
  !  rd == double (8-byte)
  !  re == long double, currently not supported

  !> @cond BUD_DEVELOPER

  integer, parameter :: BUD_CC2(rr,BUD_PRECISION_SUFFIX) = selected_real_kind(p=6) ! single (real*4)
  integer, parameter :: BUD_CC2(rd,BUD_PRECISION_SUFFIX) = selected_real_kind(p=15) ! double (real*8)
  !integer, private, parameter :: BUD_CC2(re,BUD_PRECISION_SUFFIX) = selected_real_kind(p=18)

  integer, parameter :: BUD_CC2(is,BUD_PRECISION_SUFFIX) = selected_int_kind(4) ! short (integer*2)
  integer, parameter :: BUD_CC2(ii,BUD_PRECISION_SUFFIX) = selected_int_kind(9) ! int (integer*4)
  integer, parameter :: BUD_CC2(il,BUD_PRECISION_SUFFIX) = selected_int_kind(18) ! long (integer*8)

#undef BUD_PRECISION_SUFFIX

  !> @endcond BUD_DEVELOPER


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

