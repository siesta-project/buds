  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup sm Sparse matrix
!! @ingroup bud-intrinsic
!!
!! @{

!> @defgroup sm-csr Compressed Sparse Row (CSR)
!!
!! A sparse matrix implementation.
!!
!! This is the indices for an actual sparse matrix
!! and does not contain the actual sparse data.
!!
!! @{


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iSM_CSR)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iSM_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,iSM_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
#include "SM_CSR.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lSM_CSR)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lSM_CSR)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,lSM_CSR)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
#include "SM_CSR.inc"
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

