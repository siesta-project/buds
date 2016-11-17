  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Get default commands
#include "bud_utils.inc"

  ! This module contains common variables that may be
  ! accessed in the sparse matrix modules.

#define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,SM_common)

module BUD_MOD_NAME

  implicit none

  ! Constants defined to be used as error messages
  !

  !> Errors with a negative number refers to the `-err` element
  !!
  !! This may mean that the entry is beyond the pattern size.

  !> Definition of no errors in the call
  integer, parameter, public :: SM_NONE = 0

  !> Signals two or more equivalent sparse elements
  integer, parameter, public :: SM_ENTRY_MULTIPLE = 1

  !> The input argument that is wrong by `err-SM_INPUT`
  !!
  !! This constant is an offset in the error number that
  !! refers to the input parameter `err - SM_INPUT`.
  !! I.e. if `err = SM_INPUT + 2`, the second argument
  !! is erroneous.
  integer, parameter, public :: SM_INPUT = 1000

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
