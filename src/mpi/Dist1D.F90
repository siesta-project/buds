  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

! This bud is not able to read/write itself.
#define BUD_NO_IO

!> @addtogroup bud-intrinsic

!> @defgroup dist-1d Parallel distribution (1D)
!! @ingroup bud-intrinsic
!!
!! A distribution implementation of various kinds.
!!
!! @{

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iDist1D)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iDist1D)
# define BUD_TYPE_VAR integer
# define BUD_PREC ii_
# define BUD_MPI_PREC MPI_Integer
#include "Dist1D.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lDist1D)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lDist1D)
# define BUD_TYPE_VAR integer
# define BUD_PREC il_
  ! @todo Assert that a long communicator with MPI works!!
# define BUD_MPI_PREC MPI_Integer
#include "Dist1D.inc"
end module
!> @}

!> @defgroup Distribution module for all precision objects
!! @ingroup dist-1d
!! Module for making everything public
!! @{
module BUD_CC3(BUD_MOD,_,Dist1D)

  use BUD_CC3(BUD_MOD,_,iDist1D)
  use BUD_CC3(BUD_MOD,_,lDist1D)

end module
!> @}

! GROUP dist-1d
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

