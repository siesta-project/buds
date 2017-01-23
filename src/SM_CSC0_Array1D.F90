  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup sm-csc0-array CSC0 format
!! @ingroup sm-array
!! @ingroup sm-csc0
!!
!! @{


! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)


#if BUD_DIM == 1
!> @defgroup sm-csc0-array-BUD_DIM Sparse matrix
#elif BUD_DIM == 2
!> @defgroup sm-csc0-array-BUD_DIM Sparse matrix, 1 extra dimension per element
#elif BUD_DIM == 3
!> @defgroup sm-csc0-array-BUD_DIM Sparse matrix, 2 extra dimensions per element
#elif BUD_DIM == 4
!> @defgroup sm-csc0-array-BUD_DIM Sparse matrix, 3 extra dimensions per element
#elif BUD_DIM == 5
!> @defgroup sm-csc0-array-BUD_DIM Sparse matrix, 4 extra dimensions per element
#endif
!! @ingroup sm-csc0-BUD_DIM
!!
!! A sparse matrix pattern with associated sparse matrix elements.
!!
!! This @bud is fully self-contained and enables the interaction
!! with a sparse matrix and its data.
!!
!! @ref array-BUD_DIM is the used container.
!!
#if BUD_DIM > 1
!!
!! This sparse matrix may contain additional information per sparse element
!! via an additional dimension of the contained array (@iSee @ref array-BUD_DIM).
#endif
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iCSC0_b1D)
!> @defgroup BUD_MOD_NAME Logical
!! `logical` data type
!!
!! Sparsity pattern with logical content.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSC0)
  use BUD_CC3(BUD_MOD,_,bArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iCSC0_b1D)
# define BUD_TYPE_VAR logical
#undef BUD_TYPE_VAR_PREC
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSM_CSC0)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,bArray1D)
#include "SM_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iCSC0_r1D)
!> @defgroup BUD_MOD_NAME Real (float)
!! `real(selected_real_kind(6))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSC0)
  use BUD_CC3(BUD_MOD,_,rArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iCSC0_r1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSM_CSC0)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,rArray1D)
#include "SM_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iCSC0_d1D)
!> @defgroup BUD_MOD_NAME Real (double)
!! `real(selected_real_kind(15))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSC0)
  use BUD_CC3(BUD_MOD,_,dArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iCSC0_d1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSM_CSC0)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,dArray1D)
#include "SM_Array.inc"
end module
!> @}


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iCSC0_c1D)
!> @defgroup BUD_MOD_NAME Complex (float)
!! `complex(selected_real_kind(6))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSC0)
  use BUD_CC3(BUD_MOD,_,cArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iCSC0_c1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSM_CSC0)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,cArray1D)
#include "SM_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iCSC0_z1D)
!> @defgroup BUD_MOD_NAME Complex (double)
!! `complex(selected_real_kind(15))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSC0)
  use BUD_CC3(BUD_MOD,_,zArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iCSC0_z1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSM_CSC0)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,zArray1D)
#include "SM_Array.inc"
end module
!> @}


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,CSC0_1D)
!> @defgroup BUD_MOD_NAME for all data-precision CSC0 sparse matrices
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iCSC0_b1D)
  use BUD_CC3(BUD_MOD,_,iCSC0_r1D)
  use BUD_CC3(BUD_MOD,_,iCSC0_d1D)
  use BUD_CC3(BUD_MOD,_,iCSC0_c1D)
  use BUD_CC3(BUD_MOD,_,iCSC0_z1D)
end module
!> @}


! GROUP sm-csc0-array-BUD_DIM
!> @}

! GROUP sm-csc0-array
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

