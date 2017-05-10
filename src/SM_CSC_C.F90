  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup sm-csc-c CSC-C (Compressed Sparse Column C-indexed)
!! @ingroup sm
!!
!! A compressed sparse column pattern implementation using
!! C-indexing.
!! This implementation relies on every index lookup to be
!! 0-based.
!!
!! This only contains the indices for the sparse matrix, the
!! data for the sparse matrix should be contained in an additional
!! data array of the corresponding data type.
!!
!! The CSC sparsity pattern stored can be sorted in each
!! column such that the rows are consecutively aligned.
!! This will help ensure a fast access pattern in the pattern
!! with a column-based access pattern.
!!
!! \code{.f90}
!!   integer :: nc, nz, ic, idx
!!   integer, pointer BUD_FORTRAN_CONTIGUOUS :: cptr(:), row(:)
!!
!!   call attach(this, nr=nc, nz=nz, cptr=cptr, row=row)
!!
!!   do ic = 1 , nc
!!     do idx = cptr(ic) + 1 , cptr(ic+1)
!!       ! access M(row(idx)+1,ic)
!!     end do
!!   end do
!! \endcode
!!
!! There are no data-consistency checks performed (for performance
!! reasons) hence you *can* end up with multiple entries for the
!! same pattern element.
!! In such cases the developer must take care of these.
!!
!! @note
!! This sparsity pattern is constructed to conform with the
!! MKL Sparse BLAS library.
!! The sparsity pattern is 1-based and is the 3-array variant
!! of the CSC format. The 3-array variant can be used in the
!! 4-array input without changing any array elements and/or
!! extra memory allocation.
!! To be compatible with the MKL Sparse BLAS library the
!! sparsity pattern *must* contain the diagonal elements.
!!
!! @{

#define BUD_SM_CSC 0

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSM_CSC_C)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_LIST_NAME BUD_CC2(BUD_TYPE,iList)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSM_CSC_C)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SM_CS 0
# define BUD_SM_INTEROP_C 1
#include "SM_CS.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSM_CSC_C)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_LIST_NAME BUD_CC2(BUD_TYPE,lList)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSM_CSC_C)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SM_CS 0
# define BUD_SM_INTEROP_C 1
#include "SM_CS.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,SM_CSC_C)
!> @defgroup BUD_MOD_NAME all sparse matrices in CSC-C format
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSC_C)
  use BUD_CC3(BUD_MOD,_,lSM_CSC_C)
end module
!> @}

! GROUP sm-csc-c
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

