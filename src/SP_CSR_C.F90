  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup sm-csr-c CSR-C (Compressed Sparse Row C-indexed)
!! @ingroup sm
!!
!! A compressed sparse row pattern implementation using
!! C-indexing.
!! This implementation relies on every index lookup to be
!! 0-based.
!!
!! This only contains the indices for the sparse pattern, the
!! data for the sparse pattern should be contained in an additional
!! data array of the corresponding data type.
!!
!! The CSR sparsity pattern stored *must* be sorted in each
!! row such that the columns are consecutively aligned.
!! This will help ensure a fast access pattern in the pattern
!! with a row-based access pattern.
!!
!! @note The indices stored in the arrays are C-indexed
!! but retrieving a pointer through `attach` is still
!! fortran-indexed.  
!!
!! \code{.f90}
!!   integer :: nr, nz, ir, idx
!!   integer, pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), col(:)
!!
!!   call attach(this, nr=nr, nz=nz, rptr=rptr, col=col)
!!
!!   do ir = 1 , nr
!!     do idx = rptr(ir) + 1, rptr(ir+1)
!!       ! ir-1 == C-row
!!       ! col(idx) == C-column
!!       ! access M(ir-1,col(idx))
!!     end do
!!   end do
!! \endcode
!!
!! There are no data-consistency checks performed (for performance
!! reasons) hence you *can* end up with multiple entries for the
!! same pattern element.
!! In such cases the developer must take care of these.
!!
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSP_CSR_C)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSP_CSR_C)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SP_CSR_C)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SP_CSR 0
# define BUD_SP_INTEROP_C 1
#include "SP_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSP_CSR_C)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSP_CSR_C)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SP_CSR_C)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SP_CSR 0
# define BUD_SP_INTEROP_C 1
#include "SP_CSR.inc"
end module
!> @}

! GROUP sm-csr-c
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

