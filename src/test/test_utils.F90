module test_utils

# include "bud_common_declarations.inc"

  public :: tic, toc

contains

  function tic()
    real :: tic
    call cpu_time(tic)
  end function tic

  function toc()
    real :: toc
    call cpu_time(toc)
  end function toc

end module test_utils


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

