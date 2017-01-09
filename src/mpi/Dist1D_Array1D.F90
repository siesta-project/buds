  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup dist1d-array Dense distributed array
!!
!! @{


! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)


#if BUD_DIM == 1
!> @defgroup dist1d-array-BUD_DIM Distributed dense array
#elif BUD_DIM == 2
!> @defgroup dist1d-array-BUD_DIM Distributed dense array, 1 extra dimension per element
#elif BUD_DIM == 3
!> @defgroup dist1d-array-BUD_DIM Distributed dense array, 2 extra dimensions per element
#elif BUD_DIM == 4
!> @defgroup dist1d-array-BUD_DIM Distributed dense array, 3 extra dimensions per element
#elif BUD_DIM == 5
!> @defgroup dist1d-array-BUD_DIM Distributed dense array, 4 extra dimensions per element
#endif
!! @ingroup dist1d-BUD_DIM
!!
!! A distributed dense array.
!!
!! This @bud is fully self-contained and enables the interaction
!! with a dense array and its data.
!!
!! @ref array-BUD_DIM is the used container.
!!
!! @{

#define BUD_PREC ii_

! There are no IO routines, yet
#define BUD_NO_IO

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D_b1D)
!> @defgroup BUD_MOD_NAME Logical
!! `logical` data type
!!
!! Distributed dense array with logical content.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,bArray1D)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iD1D_b1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,D1D_1D)
# define BUD_TYPE_VAR logical
#undef BUD_TYPE_VAR_PREC
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,bArray1D)
#include "Dist1D_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D_r1D)
!> @defgroup BUD_MOD_NAME Real (float)
!! `real(selected_real_kind(6)` data type
!!
!! Distributed dense array with single precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,rArray1D)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iD1D_r1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,D1D_1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,rArray1D)
#include "Dist1D_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D_d1D)
!> @defgroup BUD_MOD_NAME Real (double)
!! `real(selected_real_kind(15)` data type
!!
!! Distributed dense array with double precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,dArray1D)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iD1D_d1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,D1D_1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,dArray1D)
#include "Dist1D_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D_c1D)
!> @defgroup BUD_MOD_NAME Complex (float)
!! `complex(selected_real_kind(6)` data type
!!
!! Distributed dense array with complex single precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,cArray1D)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iD1D_c1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,D1D_1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,cArray1D)
#include "Dist1D_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D_z1D)
!> @defgroup BUD_MOD_NAME Complex (double)
!! `complex(selected_real_kind(15)` data type
!!
!! Distributed dense array with complex double precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,zArray1D)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iD1D_z1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,D1D_1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,zArray1D)
#include "Dist1D_Array.inc"
end module
!> @}



! GROUP dist1d-array-BUD_DIM
!> @}

! GROUP dist1d-array
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

