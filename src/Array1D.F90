  ! @@LICENSE@@ see Copyright notice in the top-directory

! Define the number of dimensions in this Array
#define _R_DIM 1

! Get default commands
#include "refype_utils.inc"


! Logical
# define _R_MOD_NAME _R_CC3(_R_MOD,_,bArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,bArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,bArray1D)
# define _R_TYPE_VAR logical
#undef _R_TYPE_VAR_PREC
#include "Array.inc"
end module

! Integer (short)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,sArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,sArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,sArray1D)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC is_
#include "Array.inc"
end module

! Integer (int)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,iArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,iArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,iArray1D)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC ii_
#include "Array.inc"
end module

! Integer (long)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,lArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,lArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,lArray1D)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC il_
#include "Array.inc"
end module

! Real (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,rArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,rArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,rArray1D)
# define _R_TYPE_VAR real
# define _R_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

! Real (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,dArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,dArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,dArray1D)
# define _R_TYPE_VAR real
# define _R_TYPE_VAR_PREC rd_
#include "Array.inc"
end module

! Complex (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,cArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,cArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,cArray1D)
# define _R_TYPE_VAR complex
# define _R_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

! Complex (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,zArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC2(_R_TYPE,zArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,zArray1D)
# define _R_TYPE_VAR complex
# define _R_TYPE_VAR_PREC rd_
#include "Array.inc"
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

