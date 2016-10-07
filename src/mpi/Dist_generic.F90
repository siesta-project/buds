  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Get default commands
#include "bud_utils.inc"

  ! This module contains generic variables that may be
  ! accessed in the distribution modules.

#define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,Dist_generic)

module BUD_MOD_NAME

  implicit none

  !> Denote no distribution
  integer, parameter, public :: DIST_NONE = 0

  ! currently this is not enabled as an implementation
  ! is not complete. Hence it is private.
  !> User defined distribution (DIST_EXPLICIT)
  integer, parameter, private :: DIST_USER = 1

  !> Block-cyclic distribution, remainder goes to first processors in block sizes
  integer, parameter, public :: DIST_BLOCK_CYCLIC_FIRST_SPLIT = 2
  integer, parameter, public :: DIST_BLOCK_CYCLIC = DIST_BLOCK_CYCLIC_FIRST_SPLIT

  !> A simple block-partitioning (remainder to first process)
  !!
  !! This distribution can be used with the FFTW and PEXSI library.
  integer, parameter, public :: DIST_BLOCK_LAST_ALL = 3
  integer, parameter, public :: DIST_BLOCK = DIST_BLOCK_LAST_ALL

  !> A simple cyclic-partitioning (remainder to first process)
  !!
  !! This is essentially equivalent to a block-cyclic partition with
  !! block-size == 1, so no need for wrap-around.
  integer, parameter, public :: DIST_CYCLIC_FIRST = 4
  integer, parameter, public :: DIST_CYCLIC = DIST_CYCLIC_FIRST

end module


#include "bud_cleanup.inc"
  
  
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
