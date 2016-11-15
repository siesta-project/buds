#ifndef BUD_PREC
#ifdef BUD_TYPE_VAR_PREC
#define BUD_PREC BUD_TYPE_VAR_PREC
#else
#error "BUD_PREC not defined in __FILE__"
#endif
#endif

!> @param[in] n size of array to be sorted
!! @param[in] array array to be sorted
!! @param[out] sarray sorted array
pure subroutine BUD_CC2(sort_ps_,BUD_PREC)(n, array, sarray)
  integer(BUD_PREC), intent(in) :: n, array(n)
  integer(BUD_PREC), intent(out) :: sarray(n)

  ! Local variables
  integer(BUD_PREC) :: i, j, h, FM

  if ( n == 0 ) then
    return
  else if ( n == 1 ) then
    ! no sorting
    sarray = array
    return
  end if

  ! Everything else has to be checked...
  ! This sorting routine is very efficient, and it works
  sarray(1) = array(1)
  i = 2
  sort_loop: do while ( i <= n )
    if ( array(i) <= sarray(1) ) then
      !  put in front of the sorted array
      call insert_front(n,array,sarray,i,FM)
      i = i + FM
    else if ( sarray(i-1) <= array(i) ) then
      call insert_back(n,array,sarray,i,FM)
      i = i + FM
    else if ( i-1 < 35 ) then
      ! We assume that for array segments below 35 elements
      ! it will be faster to do array search
      expSearch: do j = 2 , i - 1
        ! It will always be better to search for the end
        ! of the overlapping region. i.e. less elements to move
        if ( array(i) <= sarray(j) ) then
          h = j
          call insert_mid(n,array,sarray,h,i,FM)
          i = i + FM
          exit expSearch ! exit the loop
        end if
      end do expSearch

    else

      ! search using SFIND,
      ! We are taking advantage that both sarray(1) and sarray(i-1)
      ! has been checked, hence the -1
!      call find_bin(i-2,array(i),j)
!      j = SFIND(sarray(2:i-1),array(i),NEAREST=+1) + 1

      ! Insert directly, we have found what we searched for
      call insert_mid(n,array,sarray,j,i,FM)
      i = i + FM

    end if
  end do sort_loop
!> @cond BUD_DEVELOPER
contains

  pure subroutine insert_mid(n,array,sarray,sF,sA,P)
    integer(BUD_PREC), intent(in) :: n, array(n)
    integer(BUD_PREC), intent(inout) :: sarray(n)
    ! The place where we found a sarray(sF) <= array(sA)
    integer(BUD_PREC), intent(inout) :: sF
    integer(BUD_PREC), intent(in) :: sA ! The current reached iteration in array
    integer(BUD_PREC), intent(out) :: P ! the number of inserted values

    ! The last insertion point
    integer(BUD_PREC) :: lA

    ! First we will skip to the last non-SAME value
    ! I.e. where we can do the insert in sarray
    do while ( sF < sA - 1 .and. sarray(sF) == array(sA) )
      sF = sF + 1
    end do

    ! Now sarray(sF) < array(sA)
    if ( sF >= sA )  then
      call insert_back(n,array,sarray,sA,P)
      return
    end if

    lA = sA + 1
    ! We know we are in the middle of the sort array
    ! hence, we can exploit the next element in the sorted
    ! array
    do while ( lA <= n )
      ! If the previous sarray value is larger than the insertion
      if ( sarray(sF-1)    >  array(lA - 1) ) exit
      ! If the array is not consecutive
      if ( array(lA-1) >  array(lA) )     exit
      ! If the insertion point array is not consecutive
      if ( array(lA)   >  sarray(sF) )        exit
      ! We need to ensure an overcount of 1
      lA = lA + 1
    end do

    ! The number of elements we are pushing in
    P = lA - sA
    ! We have "overcounted"
    !lA = lA - 1

    ! Copy the mid to the front of the sarray
    do lA = sA - 1 , sF , - 1
      sarray(lA+P) = sarray(lA)
    end do
    sarray(sF:sF+P-1)   = array(sA:sA+P-1)

  end subroutine insert_mid

  pure subroutine insert_front(n,array,sarray,sA,P)
    integer(BUD_PREC), intent(in) :: n, array(n)
    integer(BUD_PREC), intent(inout) :: sarray(n)
    integer(BUD_PREC), intent(in) :: sA
    integer(BUD_PREC), intent(out) :: P ! The number of Pasted values

    ! The last insertion point
    integer(BUD_PREC) :: lA, i

    i = sA + 1
    do lA = i , n
      ! if the previous element is larger than the current element
      if ( array(lA-1) >= array(lA) ) exit
      ! if the checked point is larger than the insertion point
      if ( array(lA) >= sarray(1) ) exit
    end do
    i = lA
    do lA = i , n
      if ( array(lA) /= sarray(1) ) exit
    end do


    !      lA = sA + 1
    ! Take all the values which are smaller than sarray(1)
    !      do while ( array(lA-1) < array(lA) .and. &
    !           array(lA) < sarray(1) .and. &
    !           lA <= n )
    !         lA = lA + 1
    !      end do
    ! Take all the values which are EQUAL to sarray(1)
    !      do while ( array(lA) == sarray(1) .and. lA <= n )
    !         lA = lA + 1
    !      end do

    ! Number of points found in the sort routine
    P = lA - sA

    ! Copy over the values
    do i = sA - 1 , 1 , - 1
      sarray(P+i) = sarray(i)
    end do
    sarray(1:P)        = array(sA:lA-1)

  end subroutine insert_front

  pure subroutine insert_back(n,array,sarray,sA,P)
    integer(BUD_PREC), intent(in) :: n, array(n)
    integer(BUD_PREC), intent(inout) :: sarray(n)
    integer(BUD_PREC), intent(in) :: sA
    integer(BUD_PREC), intent(out) :: P !

    ! The last insertion point
    integer(BUD_PREC) :: lA,i

    lA = sA + 1
    ! Step until sarray(sA-1) /= array(lA)
    do lA = sA + 1 , n
      if ( sarray(sA-1) /= array(lA-1) ) exit
    end do

    ! Step until the last element of sarray, sarray(sA-1), is not
    ! smaller than the array value
    i = lA
    do lA = i , n
      if ( sarray(sA-1) >= array(lA-1) ) exit
      if ( array(lA-1) >= array(lA) ) exit
    end do

    ! The number of elements we are pushing in
    P = lA - sA
    sarray(sA:sA+P-1) = array(sA:sA+P-1)

  end subroutine insert_back

!> @endcond BUD_DEVELOPER

end subroutine


!> @param[in] n number of elements in array
!! @param[inout] array sorted array on exit
pure subroutine BUD_CC2(sort_heap_,BUD_PREC)(n, array)
  integer(BUD_PREC), intent(in) :: n
  integer(BUD_PREC), intent(inout) :: array(n)

  ! Local variables for the heap-sort algorithm
  integer(BUD_PREC) :: nF, parent

  ! Create parent tree

  ! Current number of persons in the family-tree
  nF = n
  ! sift parents in the tree
  do parent = n / 2 , 1 , -1
    call siftDown(n, array, parent)
  end do

  ! Reduce the tree size, retiring its succesive patriarchs (first element)
  do nF = n-1 , 1 , -1
    parent = array(1)
    array(1) = array(nF+1)
    array(nF+1) = parent
    call siftDown(n, array, 1_ BUD_PREC)
  end do

!> @cond BUD_DEVELOPER
contains

  ! Reorders to place in tree
  pure subroutine siftDown( n, array, person )
    integer(BUD_PREC), intent(in) :: n
    integer(BUD_PREC), intent(inout) :: array(n)
    integer(BUD_PREC), intent(in) :: person

    integer(BUD_PREC) :: sw, parent, child

    ! initialize
    parent = person                    ! assume person is a parent
    child = 2 * parent                 ! first child of parent
    sw = parent                        ! sorted swap child
    do while ( child <= nF )      ! iterate the sift-down process
      ! check current child
      if ( array(sw) < array(child) ) then
        sw = child
      end if
      ! Check neighbouring child
      if ( child < nF ) then
        if ( array(sw) < array(child+1) ) then
          sw = child + 1
        end if
      end if
      if ( sw == parent ) then
        exit ! break
      else
        child = array(parent)
        array(parent) = array(sw)
        array(sw) = child
        ! update for next sift
        parent = sw
        child = sw * 2
      end if
    end do

  end subroutine siftDown
!> @endcond BUD_DEVELOPER

end subroutine


!> @param[in] n number of elements in array
!! @param[in] array unaltered array on exit
!! @param[out] pvt pivoting array such that array(pvt(1)) is the smallest number
pure subroutine BUD_CC2(sort_heap_idx_,BUD_PREC)(n, array, pvt)
  integer(BUD_PREC) ,intent(in) :: n, array(n)
  integer(BUD_PREC) ,intent(out) :: pvt(n)

  integer(BUD_PREC) :: nF, parent

  ! Construct initial pivoting array
  do nF = 1 , n
    pvt(nF) = nF
  end do

  ! Create parent tree

  ! Current number of persons in the family-tree
  nF = n
  ! sift parents in the tree
  do parent = n / 2 , 1 , -1
    call siftDown(n, array, pvt, parent)
  end do

  ! Reduce the tree size, retiring its succesive patriarchs (first element)
  do nF = n-1 , 1 , -1
    parent = pvt(1)
    pvt(1) = pvt(nF+1)
    pvt(nF+1) = parent
    call siftDown(n, array, pvt, 1_ BUD_PREC)
  end do

!> @cond BUD_DEVELOPER
contains

  pure subroutine siftDown( n, array, pvt, person )
    integer(BUD_PREC), intent(in) :: n, array(n)
    integer(BUD_PREC), intent(inout) :: pvt(n)
    integer(BUD_PREC), intent(in) :: person
    integer(BUD_PREC) :: sw, parent, child

    ! initialize
    parent = person                    ! assume person is a parent
    child = 2 * parent                 ! first child of parent
    sw = parent                        ! sorted swap child
    do while ( child <= nF )      ! iterate the sift-down process
      ! check current child
      if ( array(pvt(sw)) < array(pvt(child)) ) then
        sw = child
      end if
      ! Check neighbouring child
      if ( child < nF ) then
        if ( array(pvt(sw)) < array(pvt(child+1)) ) then
          sw = child + 1
        end if
      end if
      if ( sw == parent ) then
        exit ! break
      else
        child = pvt(parent)
        pvt(parent) = pvt(sw)
        pvt(sw) = child
        ! update for next sift
        parent = sw
        child = sw * 2
      end if
    end do

  end subroutine siftDown
!> @endcond BUD_DEVELOPER

end subroutine



!> @param[in] n size of array to sort
!! @param[inout] array on exit, the sorted array
recursive pure subroutine BUD_CC2(sort_quick_,BUD_PREC)(n, array)
  integer(BUD_PREC), intent(in) :: n
  integer(BUD_PREC), intent(inout) :: array(n)

  integer(BUD_PREC) :: div

  integer(BUD_PREC), parameter :: one = int(1, BUD_PREC)

  if ( n <= 1 ) return

  ! Retrieve the partition ID
  call partition(n, array, div)
  call BUD_CC2(sort_quick_,BUD_PREC)(div-one, array(1))
  call BUD_CC2(sort_quick_,BUD_PREC)(n-div+one, array(div))

!> @cond BUD_DEVELOPER
contains

  ! Partition an array by swapping
  pure subroutine partition(n, array, mark)
    integer(BUD_PREC), intent(in) :: n
    integer(BUD_PREC), intent(inout) :: array(n)
    integer(BUD_PREC), intent(out) :: mark

    integer(BUD_PREC) :: i, j, x, t

    integer(BUD_PREC), parameter :: one = int(1, BUD_PREC)

    i = n / 2
    ! Find the median of the searched elements
    if ( array(1) < array(n) ) then
      if ( array(n) < array(i) ) then
        x = array(n)
      else if ( array(1) < array(i) ) then
        x = array(i)
      else
        x = array(1)
      end if
    else !if ( array(n) < array(1) )
      if ( array(1) < array(i) ) then
        x = array(1)
      else if ( array(n) < array(i) ) then
        x = array(i)
      else
        x = array(n)
      end if
    end if

    ! Starting counters..
    i = 0
    j = n + ONE

    do
      ! find point from below which is
      ! above median
      j = j - ONE
      do while ( j > 0 )
        if ( array(j) <= x ) exit
        j = j - ONE
      end do

      i = i + ONE
      do while ( i < n )
        if ( array(i) >= x ) exit
        i = i + ONE
      end do

      if ( i < j ) then
        ! exchange array(i) and array(j)
        t = array(i)
        array(i) = array(j)
        array(j) = t
      else if ( i == j ) then
        mark = i + ONE
        return
      else
        mark = i
        return
      end if
    end do

  end subroutine partition
!> @endcond BUD_DEVELOPER

end subroutine

!> @param[in] n size of array to sort
!! @param[in] array array to retrieve the pivoting table of
!! @param[out] pvt pivoting array for converting array to a sorted array
pure subroutine BUD_CC2(sort_quick_idx_,BUD_PREC)(n, array, pvt)
  integer(BUD_PREC), intent(in) :: n
  integer(BUD_PREC), intent(in) :: array(n)
  integer(BUD_PREC), intent(out) :: pvt(n)

  integer(BUD_PREC) :: i

  if ( n <= 1 ) return

  ! initialize the pivoting table
  do i = 1 , n
    pvt(i) = i
  end do

#define BUD_FIND_LOCAL BUD_CC2(internal_quick_,BUD_PREC)

  ! call actual algorithm
  call BUD_FIND_LOCAL (n, array, n, pvt)

!> @cond BUD_DEVELOPER
contains

  recursive pure subroutine BUD_FIND_LOCAL (n, array, m, pvt)
    integer(BUD_PREC), intent(in) :: n
    integer(BUD_PREC), intent(in) :: array(n)
    integer(BUD_PREC), intent(in) :: m
    integer(BUD_PREC), intent(out) :: pvt(m)

    integer(BUD_PREC) :: div

    if ( m <= 1 ) return

    ! Retrieve the partition ID
    call partition(n, array, m, pvt, div)
    call BUD_FIND_LOCAL (n, array(1), div-int(1,BUD_PREC), pvt(1))
    call BUD_FIND_LOCAL (n, array(1), m-div, pvt(div))

  end subroutine

#undef BUD_FIND_LOCAL

  ! Partition an array by swapping
  pure subroutine partition(n, array, m, pvt, mark)
    integer(BUD_PREC), intent(in) :: n
    integer(BUD_PREC), intent(in) :: array(n)
    integer(BUD_PREC), intent(in) :: m
    integer(BUD_PREC), intent(inout) :: pvt(m)
    integer(BUD_PREC), intent(out) :: mark

    integer(BUD_PREC) :: i, j, t, x

    i = pvt(1)
    j = pvt(m)
    t = pvt(m/2)

    ! Find the median of the searched elements
    if ( array(i) < array(j) ) then
      if ( array(j) < array(t) ) then
        x = array(j)
      else if ( array(i) < array(t) ) then
        x = array(t)
      else
        x = array(i)
      end if
    else !if ( array(j) < array(i) )
      if ( array(i) < array(t) ) then
        x = array(i)
      else if ( array(j) < array(t) ) then
        x = array(t)
      else
        x = array(j)
      end if
    end if

    ! Starting counters..
    i = 0
    j = m + 1

    do
      ! find point from below which is
      ! above median
      j = j - 1
      do while ( j > 0 )
        if ( array(pvt(j)) <= x ) exit
        j = j - 1
      end do

      i = i + 1
      do while ( i < m )
        if ( array(pvt(i)) >= x ) exit
        i = i + 1
      end do

      if ( i < j ) then
        ! exchange pvt(i) and pvt(j)
        t = pvt(i)
        pvt(i) = pvt(j)
        pvt(j) = t
      else if ( i == j ) then
        mark = i + 1
        return
      else
        mark = i
        return
      end if
    end do

  end subroutine partition
!> @endcond BUD_DEVELOPER

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

