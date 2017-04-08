! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup bud-intrinsic Intrinsic @buds

!> @defgroup fs-c2-array Finite stack of collection with 2 Arrays
!! @ingroup bud-intrinsic
!!
!! @bud of finite stack with a collection.
!! The collection contains two Array @buds.
!!
!! @{

! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)

!> @defgroup fs-c2-array-BUD_DIM Array BUD_DIMD
!! Finite stack of collection of Array with dimension BUD_DIM.
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,sArray1D)
!> @defgroup BUD_MOD_NAME Integer (short)
!! `integer(selected_int_kind(4))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,sArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,sArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,sArray1D)
#include "bud_finitestack.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,iArray1D)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,iArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,iArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,iArray1D)
#include "bud_finitestack.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,lArray1D)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,lArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,lArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,lArray1D)
#include "bud_finitestack.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,rArray1D)
!> @defgroup BUD_MOD_NAME Real (float)
!! `real(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,rArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,rArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,rArray1D)
#include "bud_finitestack.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,dArray1D)
!> @defgroup BUD_MOD_NAME Real (double)
!! `real(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,dArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,dArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,dArray1D)
#include "bud_finitestack.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,cArray1D)
!> @defgroup BUD_MOD_NAME Complex (float)
!! `complex(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,cArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,cArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,cArray1D)
#include "bud_finitestack.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,zArray1D)
!> @defgroup BUD_MOD_NAME Complex (double)
!! `complex(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_C2_,zArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,zArray1D)
# define BUD_FSTACK_TYPE BUD_CC3(BUD_TYPE,C2_,zArray1D)
#include "bud_finitestack.inc"
end module
!> @}

! GROUP fs-c2-array-BUD_DIM
!> @}

! GROUP fs-c2-array
!> @}


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

