module test_utils

  public :: tic, toc

contains

  subroutine start_test(name, io)
    character(len=*), intent(in) :: name
    logical, intent(in), optional :: io
    character(len=64) :: fmt
    integer :: full
    integer :: n
    if ( present(io) ) then
      if ( .not. io ) return
    end if
    full = 30 * 2 + 15
    n = (full - len(name)) / 2 - 2
    do while ( n * 2 + len(name) + 2 < full )
      n = n + 1
    end do
    write(*,'(3a)') repeat('#',30),' STARTING TEST ',repeat('#',30)
    write(*,'(2(a,tr1),a)') repeat('>',n),name,repeat('<',n)
  end subroutine start_test

  subroutine end_test(name, io)
    character(len=*), intent(in) :: name
    logical, intent(in), optional :: io
    if ( present(io) ) then
      if ( .not. io ) return
    end if
    write(*,'(3a)') repeat('#',30),'  ENDING TEST  ',repeat('#',30)
  end subroutine end_test

  function tic()
    real :: tic
    call cpu_time(tic)
  end function tic

  function toc()
    real :: toc
    call cpu_time(toc)
  end function toc

end module test_utils


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

