#ifndef _R_PREC
#ifdef _R_TYPE_VAR_PREC
#define _R_PREC _R_TYPE_VAR_PREC
#else
#error "_R_PREC not defined in FIND.inc"
#endif
#endif

#undef _R_FIND
#define _R_FIND _R_CC2(binary_find_, _R_PREC)

!> Searches for a specific integer value in a sorted array.
!!
!! This is an efficient binary search algorithm for searching
!! for specific elements in an integer array.
!!
!! @param[in] n number of elements in `array`
!! @param[in] array the searced array
!! @param[in] val value to search for in array
!! @param[out] idx index in array which contains `val`, if non-existing `val=-1`
pure subroutine _R_FIND(n, array, val, idx)
  integer(_R_PREC), intent(in) :: n, array(n), val
  integer(_R_PREC), intent(out) :: idx

  ! Local variables for doing the binary search
  integer :: a, b, d, i

  ! Initialize to default value
  idx = -1
  if ( n == 0 ) return
  
  ! The two easiest cases, i.e. they are not in the array...
  if ( val < array(1) ) then
    return
  else if ( val == array(1) ) then
    idx = 1
    return
  else if ( array(n) < val ) then
    return
  else if ( val == array(n) ) then
    idx = n
    return
  end if
  
  ! If it is size 1 or 2, we can immediately return,
  ! we have already checked the first/last index
  if ( n <= 2 ) return

  a = 1 
  b = n
  idx = 1
  d = n

  do while ( a + 1 < b .or. idx + 1 < d ) 
    i = (a+b)/2
    if ( array(i) < val ) then
      a = i
    else
      b = i
    end if
    i = (idx+d)/2
    if ( array(i) <= val ) then
      idx = i
    else
      d = i
    end if
  end do
  
  if ( b == idx .and. array(idx) /= val ) idx = -1
  
end subroutine

#undef _R_PREC

! project-refype -- local file settings
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

