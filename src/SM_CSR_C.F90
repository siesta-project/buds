  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup sm Sparse matrix
!! @ingroup bud-intrinsic
!!
!! @{

!> @defgroup sm-csr-c Compressed Sparse Row (CSR C-indexed)
!!
!! A compressed sparse row matrix implementation using
!! C-indexing.
!! This implementation relies on every index lookup to be
!! 0-based.
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
!!   integer, pointer, contiguous :: rptr(:), col(:)
!!   call attach(this, nr=nr, nz=nz, rptr, col)
!!
!!   do ir = 1 , nr
!!     do idx = rptr(ir) + 1, rptr(ir+1)
!!       ! access M(ir-1,col(idx))
!!     end do
!!   end do
!! \endcode
!!
!! There are no data-consistency checkings performed (for performance
!! reasons) hence you *can* end up with multiple entries for the
!! same matrix element.
!! In such cases the developer must take care of these.
!!
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSM_CSR_C)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSM_CSR_C)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,iSM_CSR_C)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SM_CSR 0
# define BUD_SM_INTEROP_C 1
#include "SM_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSM_CSR_C)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSM_CSR_C)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,lSM_CSR_C)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SM_CSR 0
# define BUD_SM_INTEROP_C 1
#include "SM_CSR.inc"
end module
!> @}

! GROUP sm-csr-c
!> @}

! GROUP sm
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
