  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"


! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)

!> @defgroup array-BUD_DIM Array BUD_DIMD
!! @ingroup array
!! Array of dimension BUD_DIM.
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,bArray1D)
!> @defgroup BUD_MOD_NAME Logical
!! `logical` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,bArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR logical
#undef BUD_TYPE_VAR_PREC
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,sArray1D)
!> @defgroup BUD_MOD_NAME Integer (short)
!! `integer(selected_int_kind(4))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,sArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC is_
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iArray1D)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lArray1D)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,rArray1D)
!> @defgroup BUD_MOD_NAME Real (float)
!! `real(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,rArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,dArray1D)
!> @defgroup BUD_MOD_NAME Real (double)
!! `real(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,dArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,cArray1D)
!> @defgroup BUD_MOD_NAME Complex (float)
!! `complex(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,cArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
#include "Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,zArray1D)
!> @defgroup BUD_MOD_NAME Complex (double)
!! `complex(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,zArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Array)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
#include "Array.inc"
end module
!> @}


!> @defgroup Array module for all different precision objects
!! @ingroup array-BUD_DIM
!! Module for making everything public
!! @{
module BUD_CC3(BUD_MOD,_,Array1D)

  use BUD_CC3(BUD_MOD,_,sArray1D)
  use BUD_CC3(BUD_MOD,_,iArray1D)
  use BUD_CC3(BUD_MOD,_,lArray1D)
  use BUD_CC3(BUD_MOD,_,rArray1D)
  use BUD_CC3(BUD_MOD,_,dArray1D)
  use BUD_CC3(BUD_MOD,_,cArray1D)
  use BUD_CC3(BUD_MOD,_,zArray1D)

end module
!> @}


! GROUP array-BUD_DIM
!> @}


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

