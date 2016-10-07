program test_array

  use test_utils

  use bud_iArray1D
  use bud_iArray2D
  use bud_iArray3D

  implicit none

  call t_1
  write(*,*)
  call t_2
  write(*,*)
  call t_3

contains

  subroutine t_1()
    type(iArray1D) :: a, b, c
    integer, pointer :: i(:)
    ! Try and create a new array
    call new(a,10)
    ! Try to retrieve the pointer
    i => array_p(a)
    call init_value(a, 0)
    b = a
    ! Print a (check double references)
    call print(a)
    ! Print b (check double references)
    call print(b)
    call delete(b)
    call copy(a, c)
    ! These should both contain only one reference
    call print(a)
    call print(c)
    ! try and delete the routines
    call delete(c)
    call delete(a)
    ! try and delete an already deleted object
    call delete(a)
    ! print not initialized
    call print(a)
    call print(c)
  end subroutine t_1

  subroutine t_2()
    type(iArray2D) :: a, b, c
    integer, pointer :: i(:,:)
    call new(a,10,20)
    i => array_p(a)
    call init_value(a, 0)
    b = a
    call print(a)
    call print(b)
    call delete(b)
    call copy(a, c)
    call print(a)
    call print(c)
    call delete(c)
    call delete(a)
    call delete(a)
    call print(a)
    call print(c)
  end subroutine t_2

  subroutine t_3()
    type(iArray3D) :: a, b, c
    integer, pointer :: i(:,:,:)
    call new(a,10,20,30)
    i => array_p(a)
    call init_value(a, 0)
    b = a
    call print(a)
    call print(b)
    call delete(b)
    call copy(a, c)
    call print(a)
    call print(c)
    call delete(c)
    call delete(a)
    call delete(a)
    call print(a)
    call print(c)
  end subroutine t_3

end program test_array

