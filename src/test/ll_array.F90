program test_ll_array

  use test_utils

  use bud_iArray1D
  use bud_iArray2D
  use bud_iArray3D

  use bud_LL_iArray1D
  use bud_LL_iArray2D
  use bud_LL_iArray3D

  type(iArray1D) :: a1, b1, c1
  type(iArray2D) :: a2, b2, c2
  type(iArray3D) :: a3, b3, c3

  type(LL_iArray1D) :: la1, lb1, lc1
  type(LL_iArray2D) :: la2, lb2, lc2
  type(LL_iArray3D) :: la3, lb3, lc3

  call start_test('LL_ARRAY-1D')
  call t_1
  call end_test('LL_ARRAY-1D')

contains

  subroutine t_1()
    integer :: i
    call new(a1,10)
    call add_node(la1, a1)
    call delete(a1)
    call new(a1,10)
    call add_node(la1, a1)
    call delete(a1)
    call add_node(la1)
    call new(a1,10)
    call add_node(la1, a1)
    call delete(a1)
    call print(la1)

    call set_head(la1, 1)

    ! create an iterator
    call delete(lb1)
    i = 0
    do while ( itt_step(la1, lb1) )
       call get_data(lb1, b1)
       i = i + 1
       print '(a,i0)','Element: ',i
       call print(b1)
    end do
    call print(lb1)
    call delete(b1)

    ! extract a sub-node thus having two linked
    ! lists
    call get_node(la1, b1, 1)
    call print(la1)
    call print(b1)
    call delete(b1)

    ! delete linked-list
    call delete(la1)
    call print(lb1)
    call delete(lb1)
    call print(la1)
    call print(lb1)

  end subroutine t_1

end program test_ll_array

