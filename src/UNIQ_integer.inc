#ifndef BUD_PREC
#ifdef BUD_TYPE_VAR_PREC
#define BUD_PREC BUD_TYPE_VAR_PREC
#else
#error "BUD_PREC not defined in __FILE__"
#endif
#endif


!> Counting the number of unique value for a sorted array
!!
!! @param[in] n size of array 
!! @param[in] array sorted array where we count unique numbers
!! @param[out] nuniq number of unique values in sorted array
pure subroutine BUD_CC2(nuniq_sorted_,BUD_PREC)(n, array, nuniq)
  integer(BUD_PREC), intent(in) :: n, array(n)
  integer(BUD_PREC), intent(out) :: nuniq

  integer(BUD_PREC) :: i

  nuniq = 0
  if ( n == 0 ) return

  nuniq = 1
  do i = 2 , n
    if ( array(i-1) /= array(i) ) then
      nuniq = nuniq + 1
    end if
  end do
  
end subroutine

!> Counting the number of unique value for a sorted array
!!
!! @param[in] n size of array 
!! @param[in] array sorted array where we count unique numbers
!! @param[out] nuniq number of unique values in sorted array
pure subroutine BUD_CC2(nuniq_,BUD_PREC)(n, array, nuniq)
  integer(BUD_PREC), intent(in) :: n, array(n)
  integer(BUD_PREC), intent(out) :: nuniq

  ! Temporary array for pivoting element
  integer(BUD_PREC), allocatable :: pvt(:)
  integer(BUD_PREC) :: i

  nuniq = 0
  if ( n == 0 ) return

  ! Create pivoting table
  allocate(pvt(n))
  call sort_quick(n, array, pvt)

  nuniq = 1
  do i = 2 , n
    if ( array(pvt(i-1)) /= array(pvt(i)) ) then
      nuniq = nuniq + 1
    end if
  end do

  deallocate(pvt)
  
end subroutine

!> Counting the number of unique value for a sorted array
!!
!! @param[in] n size of array 
!! @param[in] array sorted array where we count unique numbers
!! @param[in] pvt pivoting array that creates a sorted array
!! @param[out] nuniq number of unique values in sorted array
pure subroutine BUD_CC2(nuniq_pvt_,BUD_PREC)(n, array, pvt, nuniq)
  integer(BUD_PREC), intent(in) :: n, array(n), pvt(n)
  integer(BUD_PREC), intent(out) :: nuniq

  ! Temporary array for pivoting element
  integer(BUD_PREC) :: i

  nuniq = 0
  if ( n == 0 ) return

  nuniq = 1
  do i = 2 , n
    if ( array(pvt(i-1)) /= array(pvt(i)) ) then
      nuniq = nuniq + 1
    end if
  end do

end subroutine


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


