program test_array

  use test_utils

  use mryp_iArray1D
  use mryp_iArray2D
  use mryp_iArray3D

  type(iArray1D) :: a1, b1, c1
  type(iArray2D) :: a2, b2, c2
  type(iArray3D) :: a3, b3, c3

  call t_1
  write(*,*)
  call t_2
  write(*,*)
  call t_3

contains

  subroutine t_1()
    call new(a1,10)
    b1 = a1
    call print(a1)
    call print(b1)
    call delete(b1)
    c1 = a1
    call print(c1)
    call delete(c1)
    call delete(a1)
    call print(a1)
  end subroutine t_1

  subroutine t_2()
    call new(a2,10,10)
    b2 = a2
    call print(a2)
    call print(b2)
    call delete(b2)
    c2 = a2
    call print(c2)
    call delete(c2)
    call delete(a2)
    call print(a2)
  end subroutine t_2

  subroutine t_3()
    call new(a3,10,10,10)
    b3 = a3
    call print(a3)
    call print(b3)
    call delete(b3)
    c3 = a3
    call print(c3)
    call delete(c3)
    call delete(a3)
    call print(a3)
  end subroutine t_3

end program test_array

