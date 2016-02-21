  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "refype_utils.inc"

! Integer (int)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,iSM_CSR)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,iSM_CSR)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,iSM_CSR)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC ii_
#include "SM_CSR.inc"
end module

! Integer (long)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,lSM_CSR)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,lSM_CSR)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,lSM_CSR)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC il_
#include "SM_CSR.inc"
end module


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

