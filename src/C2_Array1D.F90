  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup bud-intrinsic Intrinsic @buds

!> @defgroup c2-array Collection of 2 arrays
!! @ingroup bud-intrinsic
!!
!! @bud containing a collection of 2 Array @buds.
!!
!! @{

! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)

!> @defgroup c2-array-BUD_DIM Collection of 2 Array BUD_DIMD
!! Collection of 2 @bud Arrays of dimension BUD_DIM.
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,bArray1D)
!> @defgroup BUD_MOD_NAME Logical
!! `logical` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,bArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,bArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,bArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,bArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,bArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,sArray1D)
!> @defgroup BUD_MOD_NAME Integer (short)
!! `integer(selected_int_kind(4))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,sArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,sArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,sArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,sArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,sArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,iArray1D)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,iArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,iArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,iArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,lArray1D)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,lArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,lArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,lArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,lArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,lArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,rArray1D)
!> @defgroup BUD_MOD_NAME Real (float)
!! `real(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,rArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,rArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,rArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,rArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,rArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,dArray1D)
!> @defgroup BUD_MOD_NAME Real (double)
!! `real(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,dArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,dArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,dArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,dArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,dArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,cArray1D)
!> @defgroup BUD_MOD_NAME Complex (float)
!! `complex(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,cArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,cArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,cArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,cArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,cArray1D)
#include "C2_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,zArray1D)
!> @defgroup BUD_MOD_NAME Complex (double)
!! `complex(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,zArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,zArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,zArray1D)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,zArray1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,zArray1D)
#include "C2_Array.inc"
end module
!> @}

! GROUP c2-array-BUD_DIM
!> @}

! GROUP c2-array
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

