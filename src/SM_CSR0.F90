  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup sm-csr0 CSR0 (Compressed Sparse Row 0-based row pointer)
!! @ingroup sm
!!
!! A compressed sparse row matrix implementation using
!! a 0-based row-pointer.
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
!!     do idx = rptr(ir) + 1, rptr(ir+1)
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
!! This sparsity pattern is a slight variation of the
!! regular CSR format.
!! Currently these software packages relies on this
!! special format:
!!   - [SIESTA](http://departments.icmab.es/leem/siesta/)
!!
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSM_CSR0)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_LIST_NAME BUD_CC2(BUD_TYPE,iList)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSM_CSR0)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SM_CSR 1
#include "SM_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSM_CSR0)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_LIST_NAME BUD_CC2(BUD_TYPE,lList)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSM_CSR0)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SM_CSR 1
#include "SM_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,SM_CSR0)
!> @defgroup BUD_MOD_NAME all sparse matrices in CSR0 format
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iSM_CSR0)
  use BUD_CC3(BUD_MOD,_,lSM_CSR0)
end module
!> @}

! GROUP sm-csr0
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

