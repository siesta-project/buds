#ifndef BUD_PREC
#ifdef BUD_TYPE_VAR_PREC
#define BUD_PREC BUD_TYPE_VAR_PREC
#else
#error "BUD_PREC not defined in __FILE__"
#endif
#endif

#undef BUD_FIND
#define BUD_FIND BUD_CC2(find_bin_, BUD_PREC)

!> @param[in] n number of elements in `array`
!! @param[in] array the searced array
!! @param[in] val value to search for in array
!! @param[out] idx index in array which contains `val`, if non-existing `val=-1`
pure subroutine BUD_FIND (n, array, val, idx)
  integer(BUD_PREC), intent(in) :: n, array(n), val
  integer(BUD_PREC), intent(out) :: idx

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

#undef BUD_FIND
#define BUD_FIND BUD_CC2(find_binheur_, BUD_PREC)

!> @param[in] n number of elements in `array`
!! @param[in] array the searced array
!! @param[in] val value to search for in array
!! @param[out] idx index in array which contains `val`, if non-existing `val=-1`
pure subroutine BUD_FIND (n, array, val, idx)
  integer(BUD_PREC), intent(in) :: n, array(n), val
  integer(BUD_PREC), intent(out) :: idx

  ! Used internal variables
  integer(BUD_PREC) :: h, FM
  
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
  
  ! An *advanced* search algorithm...
  
  ! Search the sorted array for an entry
  ! We know it *must* have one
  h = n / 2
  idx = h ! Start in the middle
  ! The integer correction (due to round of errors when 
  ! calculating the new half...
  FM = MOD(h,2)
  do while ( h > 0 ) ! While we are still searching...

    if ( h >= 3 ) then
      h = h + FM
      ! This will only add 1 every other time 
      FM = MOD(h,2)
    end if

    ! integer division is fast
    h = h / 2

    if ( val < array(idx) ) then
      ! the value we search for is smaller than 
      ! the current checked value, hence we step back
      idx = idx - h
    else if ( array(idx) < val ) then
      ! the value we search for is larger than 
      ! the current checked value, hence we step forward
      idx = idx + h
    else
      ! We know EXACTLY where we are...
      return
      ! We found it!!!
    end if
  end do

  ! We need to ensure a range to search in
  ! The missing integers are *only* necesseary when 
  ! the search pattern is in the same direction.
  ! This can easily be verified...
  h  = 1 + FM

  ! The missing integer count ensures the correct range
  FM = max(idx - h, 1 )
  h  = min(idx + h, n)

  ! The index will *most* likely be close to 'i'
  ! Hence we start by searching around it
  ! However, we know that val /= array(i)

  do idx = FM,  h 
    if ( val == array(idx) ) return
  end do

  ! Default value is *not found*
  idx = -1

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

