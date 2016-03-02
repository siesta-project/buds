  ! @@LICENSE@@ see Copyright notice in the top-directory

! Define the number of dimensions in this Array
#define BUD_DIM 1

! Get default commands
#include "bud_utils.inc"


! Logical
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,bArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,bArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,bArray1D)
# define BUD_TYPE_VAR logical
#undef BUD_TYPE_VAR_PREC
#include "Array.inc"
end module

! Integer (short)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,sArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,sArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,sArray1D)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC is_
#include "Array.inc"
end module

! Integer (int)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,iArray1D)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
#include "Array.inc"
end module

! Integer (long)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,lArray1D)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
#include "Array.inc"
end module

! Real (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,rArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,rArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,rArray1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

! Real (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,dArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,dArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,dArray1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
#include "Array.inc"
end module

! Complex (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,cArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,cArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,cArray1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

! Complex (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,zArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,zArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,zArray1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
#include "Array.inc"
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

