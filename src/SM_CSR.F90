  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!!$! Integer (short)
!!$# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,sSM_CSR)
!!$!> @file BUD_MOD_NAME
!!$module BUD_MOD_NAME
!!$# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,sSM_CSR)
!!$# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,sSM_CSR)
!!$# define BUD_TYPE_VAR integer
!!$# define BUD_TYPE_VAR_PREC is_
!!$#include "SM_CSR.inc"
!!$end module

! Integer (int)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSM_CSR)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSM_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,iSM_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
#include "SM_CSR.inc"
end module

! Integer (long)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSM_CSR)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSM_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,lSM_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
#include "SM_CSR.inc"
end module


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

