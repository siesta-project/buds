  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup bud-intrinsic

!> @defgroup dist-1d-sm-csr0 Parallel distribution (1D) with associated sparse matrix
!! @ingroup bud-intrinsic
!! @ingroup dist-1d-sm
!!
!! A distribution implementation with sparse matrix
!!
!! @{

! Define the sparse matrix stuff
#define BUD_SM_CSR 1
#include "SM.inc"

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D_CSR0)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,iSM_CSR0)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iD1D_CSR0)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Dist_CSR0)
# define BUD_COLL_1 BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_COLL_2 BUD_CC2(BUD_TYPE,iSM_CSR0)
# define BUD_TYPE_VAR integer
# define BUD_PREC ii_
# define BUD_MPI_PREC MPI_Integer
#include "Dist1D_SM.inc"
end module
!> @}

!!$# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lDist1D_CSR0)
!!$!> @defgroup BUD_MOD_NAME Integer (long)
!!$!! `integer(selected_int_kind(18))` data type
!!$!! @{
!!$module BUD_MOD_NAME
!!$  use BUD_CC3(BUD_MOD,_,lDist1D)
!!$  use BUD_CC3(BUD_MOD,_,lSM_CSR0)
!!$# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lD1D_CSR0)
!!$# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,Dist_CSR0)
!!$# define BUD_COLL_1 BUD_CC2(BUD_TYPE,lDist1D)
!!$# define BUD_COLL_2 BUD_CC2(BUD_TYPE,lSM_CSR0)
!!$# define BUD_TYPE_VAR integer
!!$# define BUD_PREC il_
!!$  ! @todo Assert that a long communicator with MPI works!!
!!$# define BUD_MPI_PREC MPI_Integer
!!$#include "Dist1D_SM.inc"
!!$end module
!!$!> @}

!> @defgroup Distribution+sparse matrix module for all precision objects of CSR0 matrix
!! @ingroup dist-1d-sm
!! Module for making everything public
!! @{
module BUD_CC3(BUD_MOD,_,Dist1D_CSR0)

  use BUD_CC3(BUD_MOD,_,iDist1D_CSR0)
!!$  use BUD_CC3(BUD_MOD,_,lDist1D_CSR0)

end module
!> @}

! GROUP dist-1d-sm-csr
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

