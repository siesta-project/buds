  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup sm Sparse pattern
!! @ingroup bud-intrinsic
!!
!! Sparse pattern for reduced storage formats of dense patterns with
!! many zero elements.
!!
!! @{

!> @defgroup sm-csr Compressed Sparse Row (CSR)
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


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSP_CSR)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSP_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SP_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SP_CSR 0
#include "SP_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSP_CSR)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSP_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SP_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SP_CSR 0
#include "SP_CSR.inc"
end module
!> @}

! GROUP sm-csr
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

