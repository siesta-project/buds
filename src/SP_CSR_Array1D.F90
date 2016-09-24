  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup bud-intrinsic Intrinsic @buds

!> @defgroup sm-array Sparse matrix (pattern and data)
!! @ingroup bud-intrinsic
!!
!! @bud containing a sparse matrix pattern _and_ the
!! associated data corresponding to the sparse matrix elements.
!!
!! From this object the full matrix may be constructed or interacted
!! with.
!!
!! @{


!> @defgroup sm-csr-array CSR format
!! @ingroup sm-csr
!!
!! @{


! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)


#if BUD_DIM == 1
!> @defgroup sm-csr-array-BUD_DIM Sparse matrix
#elif BUD_DIM == 2
!> @defgroup sm-csr-array-BUD_DIM Sparse matrix, 1 extra dimension per element
#elif BUD_DIM == 3
!> @defgroup sm-csr-array-BUD_DIM Sparse matrix, 2 extra dimensions per element
#elif BUD_DIM == 4
!> @defgroup sm-csr-array-BUD_DIM Sparse matrix, 3 extra dimensions per element
#elif BUD_DIM == 5
!> @defgroup sm-csr-array-BUD_DIM Sparse matrix, 4 extra dimensions per element
#endif
!! @ingroup sm-csr-BUD_DIM
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


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,bSM1DiCSR)
!> @defgroup BUD_MOD_NAME Logical
!! @details
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSP_CSR)
  use BUD_CC3(BUD_MOD,_,bArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,bSM1DiCSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SMDistCSR)
# define BUD_TYPE_VAR logical
#undef BUD_TYPE_VAR_PREC
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSP_CSR)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,bArray1D)
#include "SP_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,rSM1DiCSR)
!> @defgroup BUD_MOD_NAME Real (float)
!! `real(selected_real_kind(6))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSP_CSR)
  use BUD_CC3(BUD_MOD,_,rArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,rSM1DiCSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SMDistCSR)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSP_CSR)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,rArray1D)
#include "SP_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,dSM1DiCSR)
!> @defgroup BUD_MOD_NAME Real (double)
!! `real(selected_real_kind(15))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSP_CSR)
  use BUD_CC3(BUD_MOD,_,dArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,dSM1DiCSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SMDistCSR)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSP_CSR)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,dArray1D)
#include "SP_Array.inc"
end module
!> @}


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,cSM1DiCSR)
!> @defgroup BUD_MOD_NAME Complex (float)
!! `complex(selected_real_kind(6))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSP_CSR)
  use BUD_CC3(BUD_MOD,_,cArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,cSM1DiCSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SMDistCSR)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSP_CSR)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,cArray1D)
#include "SP_Array.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,zSM1DiCSR)
!> @defgroup BUD_MOD_NAME Complex (double)
!! `complex(selected_real_kind(15))` data type
!!
!! The sparsity pattern is in `integer(selected_int_kind(9))` precision.
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSP_CSR)
  use BUD_CC3(BUD_MOD,_,zArray1D)
#include "bud_common_declarations.inc"
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,zSM1DiCSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SMDistCSR)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iSP_CSR)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,zArray1D)
#include "SP_Array.inc"
end module
!> @}


! GROUP sm-csr-array-BUD_DIM
!> @}

! GROUP sm-csr-array
!> @}

! GROUP sm-array
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

