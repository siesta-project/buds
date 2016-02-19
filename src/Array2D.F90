  ! @@LICENSE@@ see Copyright notice in the top-directory

! Define the number of dimensions in this Array
#define _R_DIM 1

! Get default commands
#include "refype_utils.inc"


! Logical
# define _R_MOD_NAME _R_CC3(_R_MOD,_,bArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,bArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,bArray2D)
# define _R_TYPE_VAR logical
#undef _R_TYPE_VAR_PREC
#include "Array.inc"
end module

! Integer (short)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,sArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,sArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,sArray2D)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC is_
#include "Array.inc"
end module

! Integer (int)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,iArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,iArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,iArray2D)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC ii_
#include "Array.inc"
end module

! Integer (long)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,lArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,lArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,lArray2D)
# define _R_TYPE_VAR integer
# define _R_TYPE_VAR_PREC il_
#include "Array.inc"
end module

! Real (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,rArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,rArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,rArray2D)
# define _R_TYPE_VAR real
# define _R_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

! Real (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,dArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,dArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,dArray2D)
# define _R_TYPE_VAR real
# define _R_TYPE_VAR_PREC rd_
#include "Array.inc"
end module

! Complex (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,cArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,cArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,cArray2D)
# define _R_TYPE_VAR complex
# define _R_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

! Complex (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_,zArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_,zArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_,zArray2D)
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

