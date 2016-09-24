  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup sm Sparse pattern
!! @ingroup bud-intrinsic
!!
!! @{

!> @defgroup sm-csc0 Compressed Sparse Column (CSC 0-based column pointer)
!!
!! A compressed sparse column pattern implementation using
!! a 0-based column-pointer.
!!
!! This only contains the indices for the sparse pattern, the
!! data for the sparse pattern should be contained in an additional
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
!! Please note that in certain cases such *dublicate* entries may a
!! wanted feature.
!!
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSP_CSC0)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSP_CSC0)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SP_CSC0)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
# define BUD_SP_CSC 1
#include "SP_CSC.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSP_CSC0)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSP_CSC0)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,SP_CSC0)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
# define BUD_SP_CSC 1
#include "SP_CSC.inc"
end module
!> @}

! GROUP sm-csc0
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

