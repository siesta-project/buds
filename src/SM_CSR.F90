  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup sm-csr CSR (Compressed Sparse Row)
!! @ingroup sm
!!
!! A compressed sparse row matrix implementation.
!!
!! This only contains the indices for the sparse matrix, the
!! data for the sparse matrix should be contained in an additional
!! data array of the corresponding data type.
!!
!! The CSR sparsity pattern stored *must* be sorted in each
!! row such that the columns are consecutively aligned.
!! This will help ensure a fast access pattern in the matrix
!! with a row-based access pattern.
!!
!! \code{.f90}
!!   integer :: nr, nz, ir, idx
!!   integer, pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), col(:)
!!
!!   call attach(this, nr=nr, nz=nz, rptr=rptr, col=col)
!!
!!   do ir = 1 , nr
!!     do idx = rptr(ir) , rptr(ir+1) - 1
!!       ! access M(ir,col(idx))
!!     end do
!!   end do
!! \endcode
!!
!! There are no data-consistency checks performed (for performance
!! reasons) hence you *can* end up with multiple entries for the
!! same matrix element.
!! In such cases the developer must take care of these.
!!
!! @note
!! This sparsity pattern is constructed to conform with the
!! MKL Sparse BLAS library.
!! The sparsity pattern is 1-based and is the 3-array variant
!! of the CSR format. The 3-array variant can be used in the
!! 4-array input without changing any array elements and/or
!! extra memory allocation.
!! To be compatible with the MKL Sparse BLAS library the
!! sparsity pattern *must* contain the diagonal elements.
!!
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSM_CSR)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_LIST_NAME BUD_CC2(BUD_TYPE,iList)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSM_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SM_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SM_CSR 0
#include "SM_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSM_CSR)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_LIST_NAME BUD_CC2(BUD_TYPE,lList)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSM_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SM_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SM_CSR 0
#include "SM_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,SM_CSR)
!> @defgroup BUD_MOD_NAME all sparse matrices in CSR format
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSR)
  use BUD_CC3(BUD_MOD,_,lSM_CSR)
end module
!> @}

! GROUP sp-csr
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

